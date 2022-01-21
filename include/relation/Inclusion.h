#pragma once

#include "relation/RelationGraph.h"
#include "structure/CategoryRegister.h"
#include "structure/WordRegister.h"

namespace ieml::relation {


class InclusionAttribute: public RelationAttribute {
public:
    InclusionAttribute() {}

    virtual RelationType getRelationType() const override {return RelationType::INCLUSION;}

    virtual nlohmann::json to_json() const override {
        return {{"type", std::string(getRelationType()._to_string())}};
    }

};


void buildInclusionRelationGraph(RelationGraph&, 
                                 ieml::structure::PathTree::Register&,
                                 const ieml::structure::CategoryRegister&, 
                                 const ieml::structure::WordRegister&);

}