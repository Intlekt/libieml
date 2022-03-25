#pragma once

#include "ast/interfaces/AST.h"
#include "ast/PathNode.h"
#include "ParserContextManager.h"

#include <vector>
#include <sstream>


namespace ieml::AST {


class Path : public virtual AST {
public:
    Path(CharRange::Ptr&& char_range,
         std::vector<std::shared_ptr<PathNode>>&& path_nodes) : 
        AST(std::move(char_range)),
        path_nodes_(std::move(path_nodes)) {}

    virtual std::string to_string() const override {
        std::ostringstream os;

        for (const auto& node : path_nodes_) 
            os << '/' << node->to_string();
        
        return os.str();
    }

    ieml::structure::PathTree::Ptr check_path(parser::ParserContextManager& ctx) {
        if (path_nodes_.size() == 0) return nullptr;

        auto begin = path_nodes_.begin();
        auto end   = path_nodes_.end();

        const auto& path = (*begin)->check_path_node(ctx, structure::RoleType::ROOT, begin + 1, end);
        
        if (*path.begin() == nullptr) return nullptr;

        return *path.begin();
    }

private:
    const std::vector<std::shared_ptr<PathNode>> path_nodes_;
};


}