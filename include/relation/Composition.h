#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <enum.h>

#include "ParserContextManager.h"
#include "relation/BinaryRelation.h"

#include "structure/Path.h"
#include "structure/CategoryRegister.h"

#include "structure/HashElement.h"


namespace ieml::relation {


class CompositionNode {
public:
    class Register {
    public:
        std::shared_ptr<CompositionNode> get_or_create(const std::shared_ptr<structure::PathTree>& path_tree) {
            auto it = path_tree_map_.find(path_tree);
            if (it != path_tree_map_.end())
                return it->second;

            auto node = std::shared_ptr<CompositionNode>(new CompositionNode(path_tree, structure::hashElement(path_tree)));
            path_tree_map_.insert({path_tree, node});
            return node;
        }

        std::shared_ptr<CompositionNode> get_or_create(const std::shared_ptr<structure::Word>& word) {
            auto it = word_map_.find(word);
            if (it != word_map_.end())
                return it->second;

            auto node = std::shared_ptr<CompositionNode>(new CompositionNode(word, structure::hashElement(word)));
            word_map_.insert({word, node});
            return node;
        }
    private:
        struct WordHash_ {
            size_t operator()(const std::shared_ptr<structure::Word>& w) const {
                return std::hash<std::string>{}(w->getScript());
            }
        };
        struct WordEqual_ {
            bool operator()(const std::shared_ptr<structure::Word>& l, const std::shared_ptr<structure::Word>& r) const {
                return l->getScript() == r->getScript();
            }
        };

        std::unordered_map<std::shared_ptr<structure::PathTree>, std::shared_ptr<CompositionNode>> path_tree_map_;
        std::unordered_map<std::shared_ptr<structure::Word>, 
                           std::shared_ptr<CompositionNode>, 
                           WordHash_, 
                           WordEqual_> word_map_;
    };

    std::string getId() {return id_;};

    std::shared_ptr<structure::PathTree> getPathTree() const {return path_tree_;};
    std::shared_ptr<structure::Word>     getWord() const     {return word_;};

    bool isPathTree() const {return path_tree_ != nullptr;};

    std::string to_string() const {
        return "(id=" + id_ + " " + 
            (isPathTree() ? "Phrase=" + path_tree_->to_string() : "Word=" + word_->to_string()) + ")";
        };

private:
    CompositionNode(std::shared_ptr<structure::PathTree> path_tree, std::string id) : path_tree_(path_tree), id_(id) {}
    CompositionNode(std::shared_ptr<structure::Word>     word     , std::string id) : word_(word), id_(id) {}

    CompositionNode(const CompositionNode&) = delete;
    CompositionNode(CompositionNode&&) = delete;
    CompositionNode& operator=(const CompositionNode&) = delete;
    CompositionNode& operator=(CompositionNode&&) = delete;    

    const std::shared_ptr<structure::PathTree> path_tree_;
    const std::shared_ptr<structure::Word>     word_;

    const std::string id_;
};

BETTER_ENUM(CompositionRelationType, char, COMPOSITION_PHRASE, COMPOSITION_INFLECTION, COMPOSITION_AUXILIARY, COMPOSITION_JUNCTION, COMPOSITION_WORD);

class CompositionRelationAttribute {
public:
    CompositionRelationAttribute(
        const std::shared_ptr<structure::Path>& path
    ) : path_(path) {}

    std::string to_string() const {return  "path=" + path_->to_string();};
    std::shared_ptr<structure::Path> getPath() const {return path_;};

    std::shared_ptr<CompositionRelationAttribute> merge(const std::shared_ptr<CompositionRelationAttribute>& other) const {      
        auto current = path_;
        std::vector<std::shared_ptr<ieml::structure::Path>> paths;
        while (current != nullptr) {
            paths.push_back(current);
            current = current->getNext();
        }

        current = other->path_;
        for (int i = paths.size() - 1; i >= 0; --i) {
            current = std::make_shared<ieml::structure::Path>(paths[i]->getNode(), current);
        }

        return std::make_shared<CompositionRelationAttribute>(current);
    }

private:
    const std::shared_ptr<structure::Path> path_;
};


typedef BinaryRelation<CompositionNode, CompositionRelationAttribute, CompositionRelationType> CompositionRelation;
typedef BinaryRelationGraph<CompositionNode, CompositionRelation, CompositionRelationType> CompositionRelationGraph;

std::shared_ptr<CompositionRelationGraph> buildCompositionRelationGraph(
    CompositionNode::Register& node_register, 
    const ieml::structure::CategoryRegister& creg,
    const ieml::structure::WordRegister& wreg);

}