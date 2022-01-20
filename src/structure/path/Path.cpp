#include "structure/path/PathTree.h"
#include "structure/WordRegister.h"

#include <algorithm>


using namespace ieml::structure;


void Path::check() {
    if (next_) { 
        if(!node_->accept_next(*(next_->node_)))
            throw std::invalid_argument("Invalid path descendent for " + node_->to_string() + " got " + next_->node_->to_string());

        next_->check();
    }
}
std::string Path::to_string() const {
    auto node_s = "/" + node_->to_string();
    if (next_)
        node_s += next_->to_string();

    return node_s;
}

std::shared_ptr<Path> Path::from_string(const std::string& s, const WordRegister& ctx) {
    if (s.size() == 0) throw std::invalid_argument("Invalid path string, empty string.");
    if (s[0] != '/') throw std::invalid_argument("Invalid starting character for path string.");

    size_t end_pos, start_pos = 1;
    std::vector<std::string> sub_s;
    while ((end_pos = s.find('/', start_pos)) != std::string::npos) {
        sub_s.push_back(s.substr(start_pos, end_pos - start_pos));
        start_pos = end_pos + 1;
    }
    sub_s.push_back(s.substr(start_pos, s.size() - start_pos));

    std::vector<std::shared_ptr<PathNode>> nodes;
    for (size_t i = 0; i < sub_s.size(); i++) {
        switch(sub_s[i][0]) {
            case '#': {
                if (sub_s[i].size() != 1)
                    throw std::invalid_argument("Invalid root path node: got '" + sub_s[i] + "'.");

                nodes.push_back(std::make_shared<RootPathNode>());
                break;
            }
            case '&': {
                // junction identifier
                std::string jct_script = sub_s[i].substr(2, sub_s[i].size() - 3);
                auto junction = ctx.get_word(jct_script);

                if (junction == nullptr) 
                    throw std::invalid_argument("Invalid junction path node, undefined junction word: got '" + jct_script + "'.");
                
                if (junction->getWordType() != +WordType::JUNCTION)
                    throw std::invalid_argument("Invalid junction path node, '" + jct_script + "' is not a junction.");
                auto junction_word = std::dynamic_pointer_cast<JunctionWord>(junction);

                std::shared_ptr<JunctionPathNode> junction_node;
                if (nodes.size() == 0)                         
                    throw std::invalid_argument("Invalid path, a path cannot start with a junction path node, cannot determine junction type.");
                else {
                    auto last = *nodes.rbegin();
                    switch (last->getPathType())
                    {
                    case PathType::ROOT:
                    case PathType::JUNCTION_PHRASE_INDEX:
                        junction_node = std::make_shared<PhraseJunctionPathNode>(junction_word);
                        break;

                    case PathType::ROLE:
                    case PathType::JUNCTION_AUXILIARY_INDEX:
                        junction_node = std::make_shared<AuxiliaryJunctionPathNode>(junction_word);
                        break;

                    case PathType::AUXILIARY:
                    case PathType::JUNCTION_INFLECTION_INDEX:
                        junction_node = std::make_shared<InflectionJunctionPathNode>(junction_word);
                        break;

                    case PathType::INFLECTION:
                    case PathType::JUNCTION_CATEGORY_INDEX:
                        junction_node = std::make_shared<CategoryJunctionPathNode>(junction_word);
                        break;

                    default:
                        throw std::invalid_argument("Invalid path, a junction index path node must be preceded by a junction path node.");
                    }

                }

                nodes.push_back(junction_node);
                break;

            }
            case '[': {
                // junction index
                size_t read;
                int junction_index;
                try {
                    junction_index = std::stoi(sub_s[i].substr(1, sub_s[i].size() - 2), &read);
                } catch (std::invalid_argument) {
                    throw std::invalid_argument("Invalid junction index path node: got '" + sub_s[i] + "'.");
                }
                if (read != sub_s[i].size())
                    throw std::invalid_argument("Invalid junction index path node: got '" + sub_s[i] + "'.");

                if (nodes.size() == 0)
                    throw std::invalid_argument("Invalid path, a path cannot start with a junction index path node, cannot determine junction type.");

                auto last = *nodes.rbegin();
                std::shared_ptr<JunctionIndexPathNode> junction_node;
                switch (last->getPathType())
                {
                case PathType::JUNCTION_PHRASE:
                    junction_node = std::make_shared<PhraseJunctionIndexPathNode>(junction_index);
                    break;
                case PathType::JUNCTION_AUXILIARY:
                    junction_node = std::make_shared<AuxiliaryJunctionIndexPathNode>(junction_index);
                    break;
                case PathType::JUNCTION_INFLECTION:
                    junction_node = std::make_shared<InflectionJunctionIndexPathNode>(junction_index);
                    break;
                case PathType::JUNCTION_CATEGORY:
                    junction_node = std::make_shared<CategoryJunctionIndexPathNode>(junction_index);
                    break;
                default:
                    throw std::invalid_argument("Invalid path, a junction index path node must be preceded by a junction path node.");
                }

                nodes.push_back(junction_node);
                break;
            }
            case '*': {
                // auxiliary
                std::string aux_script = sub_s[i].substr(2, sub_s[i].size() - 3);
                auto aux = ctx.get_word(aux_script);

                if (aux == nullptr) 
                    throw std::invalid_argument("Invalid auxiliary path node, undefined auxiliary word: got '" + aux_script + "'.");
                
                if (aux->getWordType() != +WordType::AUXILIARY)
                    throw std::invalid_argument("Invalid auxiliary path node, '" + aux_script + "' is not an auxiliary.");

                nodes.push_back(std::make_shared<AuxiliaryPathNode>(std::dynamic_pointer_cast<AuxiliaryWord>(aux)));
                break;
            }
            case '~': {
                // inflection
                std::vector<std::string> inflection_scripts;
                std::string s = sub_s[i];

                size_t end_pos, start_pos = 1;
                while ((end_pos = s.find('~', start_pos)) != std::string::npos) {
                    inflection_scripts.push_back(s.substr(start_pos, end_pos - start_pos));
                    start_pos = end_pos + 1;
                }
                inflection_scripts.push_back(s.substr(start_pos, s.size() - start_pos));

                std::set<std::shared_ptr<InflectionWord>> inflection_words;
                for (auto inflection_script: inflection_scripts) {
                    auto inflection_script_ = inflection_script.substr(1, sub_s[i].size() - 2);
                    auto word = ctx.get_word(inflection_script_);

                    if (word == nullptr) 
                        throw std::invalid_argument("Invalid inflection word path node, undefined inflection word: got '" + inflection_script_ + "'.");
                
                    if (word->getWordType() != +WordType::INFLECTION)
                        throw std::invalid_argument("Invalid inflection path node, '" + inflection_script_ + "' is not an inflection.");

                    inflection_words.insert(std::dynamic_pointer_cast<InflectionWord>(word));
                }
                
                nodes.push_back(std::make_shared<InflectionPathNode>(inflection_words));
                break;
            }
            case '\'': {
                // category word
                std::string word_script = sub_s[i].substr(1, sub_s[i].size() - 2);
                auto word = ctx.resolve_category_word(word_script);

                if (word == nullptr) 
                    throw std::invalid_argument("Invalid category word path node, undefined category word: got '" + word_script + "'.");
                
                nodes.push_back(std::make_shared<WordPathNode>(std::move(word)));
                break;
            }
            default: {
                // ROLE number or invalid input
                if (sub_s[i].size() != 1)
                    throw std::invalid_argument("Invalid path node: got '" + sub_s[i] + "'.");

                size_t read;
                int role_number;
                try {
                    role_number = std::stoi(sub_s[i], &read);
                } catch (std::invalid_argument) {
                    throw std::invalid_argument("Invalid role number path node: got '" + sub_s[i] + "'.");
                }
                if (read != sub_s[i].size())
                    throw std::invalid_argument("Invalid role number path node: got '" + sub_s[i] + "'.");


                auto role_type = RoleType::_from_integral_nothrow(role_number);
                if (!role_type)
                    throw std::invalid_argument("Invalid role number path node: got '" + sub_s[i] + "'.");

                nodes.push_back(std::make_shared<RoleNumberPathNode>(*role_type));
                break;
            }
        }
    }

    std::shared_ptr<Path> c = nullptr;
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        c = std::make_shared<Path>(*it, c);
    }
    return c;
}


