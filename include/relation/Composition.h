#pragma once

#include <memory>

#include "ParserContext.h"
#include "relation/BinaryRelation.h"

#include "structure/Path.h"
#include "structure/CategoryRegister.h"

namespace ieml::relation {

class CompositionRelation : public BinaryRelation<structure::PathTree> {
public:
    CompositionRelation(const std::shared_ptr<structure::PathTree>& subject, 
                        const std::shared_ptr<structure::PathTree>& object, 
                        const std::shared_ptr<structure::Path>& path) : 
        path_(path),
        BinaryRelation<structure::PathTree>(subject, object) {}

    std::string to_string() const {
        return getSubject()->to_string() + " -> " + getObject()->to_string() + " [" + path_->to_string() + "]";
    }

    std::shared_ptr<structure::Path> getPath() const {return path_;};

    Class getClass() const { return Class::COMPOSITION;};

private:
    const std::shared_ptr<structure::Path> path_;
};

typedef BinaryRelationGraph<structure::PathTree, CompositionRelation> CompositionRelationGraph;

}