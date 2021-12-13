#pragma once

#include <memory>
#include <map>

#include "ast/interfaces/AST.h"
#include "structure/Path.h"


namespace ieml {
namespace parser {

class SourceMapping {
public:
    SourceMapping() {};

    void register_mapping(std::shared_ptr<ieml::structure::PathTree> path_tree, ieml::AST::AST* declaration) {
        map_.insert({
            path_tree,
            declaration
        });
    }

    const ieml::AST::AST* resolve_mapping(std::shared_ptr<ieml::structure::PathTree> path_tree) const {
        auto it = map_.find(path_tree);
        if (it == map_.end()) 
            return nullptr;
        return it->second;
    }

private:
    std::unordered_map<std::shared_ptr<ieml::structure::PathTree>, 
                       const ieml::AST::AST*> map_;
};

}}