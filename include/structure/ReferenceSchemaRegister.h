#pragma once


#include "structure/reference/ReferenceSchema.h"
#include "structure/path/PathTree.h"


namespace ieml::structure {

class ReferenceSchemaRegister {
public:
    ReferenceSchemaRegister() {}

    void register_schema(const std::shared_ptr<PathTree>& path_tree, ReferenceSchema&& schema) {
        schemas_.insert({path_tree, std::move(schema)});
    }

    bool is_defined(const std::shared_ptr<PathTree>& path_tree) const {
        return schemas_.find(path_tree) != schemas_.end();
    }

    const ReferenceSchema& get_schema(const std::shared_ptr<PathTree>& path_tree) const {
        const auto& it = schemas_.find(path_tree);

        if (it == schemas_.end())
            return ReferenceSchema::Empty;

        return it->second;
    }

private:    
    std::unordered_map<std::shared_ptr<PathTree>, const ReferenceSchema> schemas_;
};


}