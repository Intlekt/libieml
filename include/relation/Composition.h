#pragma once

#include <memory>

#include "ParserContext.h"
#include "relation/BinaryRelation.h"

#include "structure/Path.h"
#include "structure/CategoryRegister.h"

namespace ieml::relation {

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


typedef BinaryRelation<structure::PathTree, CompositionRelationAttribute> CompositionRelation;
typedef BinaryRelationGraph<structure::PathTree, CompositionRelation> CompositionRelationGraph;

}