#pragma once

#include "relation/RelationGraph.h"
#include "structure/CategoryRegister.h"
#include "structure/WordRegister.h"



namespace ieml::relation {

BETTER_ENUM(CompositionRelationType, char, COMPOSITION_PHRASE, COMPOSITION_INFLECTION, COMPOSITION_AUXILIARY, COMPOSITION_JUNCTION, COMPOSITION_WORD);

class CompositionAttribute: public RelationAttribute {
public:
    CompositionAttribute(std::shared_ptr<structure::Path> path,
                         CompositionRelationType cmp_type) : 
        path_(path),
        cmp_type_(cmp_type) {}

    virtual RelationType getRelationType() const override {return RelationType::COMPOSITION;};

    virtual nlohmann::json to_json() const override {
        return {
                {"type", std::string(getRelationType()._to_string())},
                {"composition_type", std::string(cmp_type_._to_string())},
                {"path", path_->to_string()}
            };
    }


    CompositionRelationType getCompositionRelationType() const {return cmp_type_;}
    std::shared_ptr<structure::Path> getCompositionPath() const {return path_;}


private:
    const std::shared_ptr<structure::Path> path_;
    const CompositionRelationType cmp_type_;
};

void buildCompositionRelationGraph(RelationGraph&, const ieml::structure::CategoryRegister&, const ieml::structure::WordRegister&);
}