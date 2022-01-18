#include "relation/Inclusion.h"


void ieml::relation::buildInclusionRelationGraph(RelationGraph& graph, 
                                                 const ieml::structure::CategoryRegister& creg, 
                                                 const ieml::structure::WordRegister& wreg) {

    auto is_phrase = [&creg](const std::shared_ptr<ieml::structure::PathTree>& t){
        return t->is_phrase() && creg.category_is_defined(t);
    };

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it)
        graph.add_node(it->first);

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it) {
        if (creg.getDefinitionType(it->first) != +ieml::structure::DefinitionType::PARADIGM) continue;

        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(is_phrase, is_phrase)) {
            graph.add_relation((Relation){
                .source=it->first,
                .target=subphrase.second,
                .attribute=std::make_shared<InclusionAttribute>()
            });
        }
    }
}
