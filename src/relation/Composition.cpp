#include "relation/BinaryRelation.h"
#include "relation/Composition.h"
#include "structure/Path.h"
#include "structure/CategoryRegister.h"

#include <memory>

using namespace ieml::relation;



std::shared_ptr<CompositionRelationGraph> ieml::relation::buildCompositionRelationGraph(
    CompositionNode::Register& node_register, 
    const ieml::structure::CategoryRegister& creg,
    const ieml::structure::WordRegister& wreg) {
    
    auto graph = std::make_shared<CompositionRelationGraph>();

    auto is_phrase = [&creg](const std::shared_ptr<ieml::structure::PathTree>& t){
        return t->is_phrase() && creg.category_is_defined(t);
    };
    // auto is_inflection = [&wreg](const std::shared_ptr<ieml::structure::PathTree>& t){
    //     return t->is_inflection() && wreg.inflection_is_defined(std::dynamic_pointer_cast<ieml::structure::InflectionPathNode>(t->getNode()));
    // };


    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it) {
        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(is_phrase, is_phrase)) {
            graph->add_relation(std::make_shared<CompositionRelation>(
                node_register.get_or_create(it->first),
                node_register.get_or_create(subphrase.second),
                std::make_shared<CompositionRelationAttribute>(subphrase.first)
            ));
        }

        // inflections
        for (auto& inflections : it->first->find_sub_tree(&ieml::structure::PathTree::is_inflection, is_phrase)) {

            for (auto& inflection: std::dynamic_pointer_cast<ieml::structure::InflectionPathNode>(inflections.second->getNode())->getWords()) {
                graph->add_relation(std::make_shared<CompositionRelation>(
                    node_register.get_or_create(it->first),
                    node_register.get_or_create(inflection),
                    std::make_shared<CompositionRelationAttribute>(inflections.first)
                ));

            }
        }

    }

    for (auto it = wreg.inflections_begin(); it != wreg.inflections_end(); ++it) {

    }

    return graph;
};

