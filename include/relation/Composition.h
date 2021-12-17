#pragma once

#include <memory>
#include <unordered_map>

#include "ParserContext.h"
#include "relation/BinaryRelation.h"

#include "structure/Path.h"
#include "structure/CategoryRegister.h"

namespace ieml::relation {


class CompositionNode {
public:
    class Register {
    public:
        std::shared_ptr<CompositionNode> get_or_create(std::shared_ptr<structure::PathTree> path_tree) {
            auto it = path_tree_map_.find(path_tree);
            if (it != path_tree_map_.end())
                return it->second;

            auto node = std::shared_ptr<CompositionNode>(new CompositionNode(path_tree, index++));
            path_tree_map_.insert({path_tree, node});
            return node;
        }

        std::shared_ptr<CompositionNode> get_or_create(std::shared_ptr<structure::Word> word) {
            auto it = word_map_.find(word);
            if (it != word_map_.end())
                return it->second;

            auto node = std::shared_ptr<CompositionNode>(new CompositionNode(word, index++));
            word_map_.insert({word, node});
            return node;
        }
    private:
        size_t index = 0;

        std::unordered_map<std::shared_ptr<structure::PathTree>, std::shared_ptr<CompositionNode>> path_tree_map_;
        std::unordered_map<std::shared_ptr<structure::Word>, std::shared_ptr<CompositionNode>> word_map_;
    };

    size_t getId() {return id_;};

private:
    CompositionNode(std::shared_ptr<structure::PathTree> path_tree, size_t id) : path_tree_(path_tree), id_(id) {}
    CompositionNode(std::shared_ptr<structure::Word>     word     , size_t id) : word_(word), id_(id) {}

    CompositionNode(const CompositionNode&) = delete;
    CompositionNode(CompositionNode&&) = delete;
    CompositionNode& operator=(const CompositionNode&) = delete;
    CompositionNode& operator=(CompositionNode&&) = delete;    

    const std::shared_ptr<structure::PathTree> path_tree_;
    const std::shared_ptr<structure::Word>     word_;

    const size_t id_;
};



class CompositionRelationAttribute {
public:
    CompositionRelationAttribute(const std::shared_ptr<structure::Path>& path) : path_(path) {}

    std::string to_string() const {return path_->to_string();};
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


typedef BinaryRelation<CompositionNode, CompositionRelationAttribute> CompositionRelation;
typedef BinaryRelationGraph<CompositionNode, CompositionRelation> CompositionRelationGraph;

std::shared_ptr<CompositionRelationGraph> buildCompositionRelationGraph(CompositionNode::Register& node_register, const ieml::structure::CategoryRegister& reg);

}