#include "relation/Inclusion.h"


void ieml::relation::buildInclusionRelationGraph(RelationGraph& graph, 
                                                 ieml::structure::PathTree::Register& register_,
                                                 const ieml::structure::CategoryRegister& creg, 
                                                 __attribute__((unused)) const ieml::structure::WordRegister& wreg) {

    auto is_phrase = [&creg](const ieml::structure::PathTree::Ptr& t){
        return t->is_phrase() && creg.category_is_defined(t);
    };

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it)
        graph.add_node(it->first);

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it) {
        if (creg.getDefinitionType(it->first) != +ieml::structure::DefinitionType::PARADIGM) continue;

        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(register_, is_phrase, is_phrase)) {
            graph.add_relation(Relation(
                it->first,
                subphrase.second,
                std::make_shared<InclusionAttribute>()
            ));
        }
    }
}
