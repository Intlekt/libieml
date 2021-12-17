#include "relation/BinaryRelation.h"
#include "relation/Composition.h"
#include "structure/Path.h"
#include "structure/CategoryRegister.h"


using namespace ieml::relation;



std::shared_ptr<CompositionRelationGraph> ieml::relation::buildCompositionRelationGraph(CompositionNode::Register& node_register, const ieml::structure::CategoryRegister& reg) {
    
    auto graph = std::make_shared<CompositionRelationGraph>();

    auto is_phrase = [&reg](const std::shared_ptr<ieml::structure::PathTree>& t){
        return t->is_phrase() && reg.category_is_defined(t);
    };

    for (auto it = reg.categories_begin(); it != reg.categories_end(); ++it) {
        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(is_phrase, is_phrase)) {
            graph->add_relation(std::make_shared<CompositionRelation>(
                node_register.get_or_create(it->first),
                node_register.get_or_create(subphrase.second),
                std::make_shared<CompositionRelationAttribute>(subphrase.first)
            ));
        }
    }

    return graph;
};

