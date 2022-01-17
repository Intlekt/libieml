#include "structure/Path.h"

#include <algorithm>


namespace ieml::structure {
    bool RootPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::ROLE:
        case PathType::JUNCTION_PHRASE:
            return true;
        default:
            return false;
        }
    };
    PathType RootPathNode::getPathType() const {return PathType::ROOT;};
    std::string RootPathNode::to_string() const { return "#"; };

    bool ParadigmPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };
    PathType ParadigmPathNode::getPathType() const {return PathType::PARADIGM;};
    std::string ParadigmPathNode::to_string() const { return "{}"; };

    bool RoleNumberPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
        case PathType::JUNCTION_INFLECTION:
        case PathType::INFLECTION:
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
            return true;
        default:
            return false;
        }
    };
    PathType RoleNumberPathNode::getPathType() const {return PathType::ROLE;};
    std::string RoleNumberPathNode::to_string() const {return std::to_string(role_type_);}

    std::string JunctionPathNode::to_string() const {
        return "&" + std::string(junction_type_->to_string());
    }
    
    bool PhraseJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_PHRASE_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType PhraseJunctionPathNode::getPathType() const {return PathType::JUNCTION_PHRASE;};

    bool AuxiliaryJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType AuxiliaryJunctionPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY;};

    bool InflectionJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLECTION_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType InflectionJunctionPathNode::getPathType() const {return PathType::JUNCTION_INFLECTION;};

    bool CategoryJunctionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY_INDEX:
            return true;
        default:
            return false;
        }
    };
    PathType CategoryJunctionPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY;};

    std::string JunctionIndexPathNode::to_string() const {
        return "[" + std::to_string(index_) + "]";;
    }
    bool PhraseJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::ROLE:
        case PathType::JUNCTION_PHRASE:
            return true;
        default:
            return false;
        }
    };
    PathType PhraseJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_PHRASE_INDEX;};

    bool AuxiliaryJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_AUXILIARY:
        case PathType::AUXILIARY:
            return true;
        default:
            return false;
        }
    };
    PathType AuxiliaryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_AUXILIARY_INDEX;};

    bool InflectionJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLECTION:
        case PathType::INFLECTION:
            return true;
        default:
            return false;
        }
    };
    PathType InflectionJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_INFLECTION_INDEX;};

    bool CategoryJunctionIndexPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };
    PathType CategoryJunctionIndexPathNode::getPathType() const {return PathType::JUNCTION_CATEGORY_INDEX;};

    bool AuxiliaryPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_INFLECTION:
        case PathType::INFLECTION:
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };
    PathType AuxiliaryPathNode::getPathType() const {return PathType::AUXILIARY;};
    std::string AuxiliaryPathNode::to_string() const {
        return "*" + auxiliary_type_->to_string();
    }

    bool InflectionPathNode::accept_next(const PathNode& next) const {
        switch (next.getPathType())
        {
        case PathType::JUNCTION_CATEGORY:
        case PathType::WORD:
        case PathType::ROOT:
            return true;
        default:
            return false;
        }
    };
    PathType InflectionPathNode::getPathType() const {return PathType::INFLECTION;};
    std::string InflectionPathNode::to_string() const {
        std::ostringstream os;
        for (auto& inflection: inflections_)
            os << "~" << inflection->to_string();
        
        return os.str();
    }

    bool WordPathNode::accept_next(const PathNode& next) const {
        return false;
    };
    PathType WordPathNode::getPathType() const {return PathType::WORD;};
    std::string WordPathNode::to_string() const {
        return word_->to_string();
    }

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

    PathTree::Set PathTree::Register::get_or_create_product(const PathNode::Set& node_set, const std::vector<PathTree::Set>& children_list) {
        size_t size = 1;
        for (auto& set : children_list) size *= set.size();

        std::vector<Set> bins(size);
        for (auto& set : children_list) {
            size_t i = 0;
            for (auto& child: set) {
                for (size_t j = 0; j < size / set.size(); ++j)
                    bins[j * set.size() + i].insert(child);

                ++i;
            }
        }

        Set res;
        for (auto& children: bins)
            for (auto& node: node_set)
                res.insert(get_or_create(node, children));

        return res;
    }


    PathTree::Set PathTree::Register::get_or_create_product(const std::shared_ptr<PathNode>& node, const std::vector<PathTree::Set>& children_list) {
        return get_or_create_product((PathNode::Set){node}, children_list);
    }

    std::shared_ptr<PathTree> PathTree::Register::get_or_create(const std::shared_ptr<PathNode>& node, const PathTree::Set& children) {
        auto key = Key(node, children);
        auto it = store_.find(key);
        if (it != store_.end()) return it->second;

        auto item = std::shared_ptr<PathTree>(new PathTree(node, children));
        store_.insert({key, item});
        return item;
    }
    std::shared_ptr<PathTree> PathTree::Register::get_or_create(const std::shared_ptr<PathNode>& node) {
        return get_or_create(node, Set{});
    }


    std::shared_ptr<PathTree> PathTree::Register::buildFromPaths(std::vector<std::shared_ptr<Path>> paths) {
        const auto node = paths[0]->getNode();
        PathType path_type(PathType::ROOT);
        if (paths[0]->getNext())
            path_type = paths[0]->getNext()->getNode()->getPathType();

        std::unordered_map<PathNode*, std::vector<std::shared_ptr<Path>>> children_paths;

        for (auto path : paths) {
            // maybe change that to test equality by value ?
            // will have issues with junction 
            if (*path->getNode() != *node)
                throw std::invalid_argument("All path does not share the same prefix: had '" + node->to_string() + "', got '" + path->getNode()->to_string() + "'.");

            auto subpath = path->getNext();
            if (subpath) {
                if (subpath->getNode()->getPathType() != path_type)
                    throw std::invalid_argument("All subpath does not share the same path type: had '" + std::string(path_type._to_string()) + "', got '" \
                                                    + std::string(subpath->getNode()->getPathType()._to_string()) + "'.");

                if (children_paths.count(subpath->getNode().get()) < 1) {
                    children_paths.insert({subpath->getNode().get(), {subpath}});
                } else {
                    children_paths[subpath->getNode().get()].push_back(subpath);
                }
            }
        }
        
        Set children;

        for (auto paths: children_paths) {
            children.insert(buildFromPaths(paths.second));
        }

        return get_or_create(node, children);
    }


    std::string PathTree::to_string() const {
        std::stringstream os;

        os << node_->to_string();

        for (auto& child: children_) {
            os << "(" << child->to_string() << ")";
        }
        return os.str();
    }

    std::vector<std::shared_ptr<PathTree>> PathTree::getChildrenAsVector() const {
        std::vector<std::shared_ptr<PathTree>> v;

        if (children_.size() == 0)
            return v;

        v.reserve(children_.size());
        for(auto it = children_.begin(); it != children_.end(); ++it ) {
            v.push_back(*it);
        }

        return v;
    };

    std::vector<PathTree::SubPathTree> PathTree::find_sub_tree(std::function<bool(const std::shared_ptr<PathTree>&)> f,
                                           std::function<bool(const std::shared_ptr<PathTree>&)> should_stop) const {
        std::vector<SubPathTree> res;
        for (auto& child: children_) {
            bool matched = f(child);
            if (matched) res.push_back(SubPathTree{std::make_shared<Path>(node_), child});
            
            if (!should_stop(child)) {
                auto child_res = child->find_sub_tree(f, should_stop);
                res.reserve(res.size() + child_res.size());

                for (auto& item: child_res) {
                    res.push_back(SubPathTree{std::make_shared<Path>(node_, item.first), item.second});
                }
            }
        }
        return res;
    };

    PathTree::Set PathTree::singular_sequences(std::shared_ptr<PathTree>& pt) {
        switch (pt->getNode()->getPathType()) {
            case PathType::ROOT:
                return {pt};
            case PathType::PARADIGM:
                return pt->getChildren();
            default:
                throw std::invalid_argument("Invalid path type for singular sequences " + std::string(pt->getNode()->getPathType()._to_string()));
        }
    };

    int PathTree::comp(const std::shared_ptr<PathNode>& nodeA, const PathTree::Set& childrenA, 
                       const std::shared_ptr<PathNode>& nodeB, const PathTree::Set& childrenB) {
        if (*nodeA == *nodeB) {
            auto it = childrenA.begin();
            auto it_r = childrenB.begin();

            for (;it != childrenA.end() && it_r != childrenB.end();) {
                if (**it != **it_r) return **it < **it_r ? -1 : 1;

                ++it;
                ++it_r;
            }

            if (it == childrenA.end() && it_r == childrenB.end()) return 0;

            return it == childrenA.end() ? -1 : 1;
        } else 
            return *nodeA < *nodeB ? -1 : 1;
    };

    int PathTree::comp_element_(const Element& o_elem) const {
        const auto& o = dynamic_cast<const PathTree&>(o_elem);
        return comp(node_, children_, o.getNode(), o.getChildren());
    };

}
