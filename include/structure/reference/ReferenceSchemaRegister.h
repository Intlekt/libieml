#pragma once


#include "structure/reference/ReferenceSchema.h"
#include "structure/path/PathTree.h"


namespace ieml::structure {

class ReferenceSchemaRegister {
public:
    /**
     * @brief Construct a new Reference Schema Register object
     * 
     * Hold the mapping between a pathTree and its possible Reference Schema
     * At the moment, only one Reference Schema is allowed per path tree.
     * 
     *       
     * 
     */
    ReferenceSchemaRegister() = default;

    ReferenceSchemaRegister(const ReferenceSchemaRegister&) = delete;
    ReferenceSchemaRegister& operator=(ReferenceSchemaRegister&) = delete;


    void register_schema(const PathTree::Ptr& path_tree, ReferenceSchema&& schema) {
        schemas_.insert({path_tree, std::move(schema)});
    }

    bool is_defined(const PathTree::Ptr& path_tree) const {
        return schemas_.find(path_tree) != schemas_.end();
    }

    const ReferenceSchema& get_schema(const PathTree::Ptr& path_tree) const {
        const auto& it = schemas_.find(path_tree);
        return it->second;
    }

private:    
    std::unordered_map<PathTree::Ptr, const ReferenceSchema> schemas_;
};


}