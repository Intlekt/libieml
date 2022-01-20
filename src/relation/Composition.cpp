#include "relation/Composition.h"
#include "structure/path/PathTree.h"
#include "structure/CategoryRegister.h"

#include <memory>

using namespace ieml::relation;



void ieml::relation::buildCompositionRelationGraph(RelationGraph& graph, 
                                                   const ieml::structure::CategoryRegister& creg, 
                                                   const ieml::structure::WordRegister& wreg) {
    

    auto is_phrase = [&creg](const std::shared_ptr<ieml::structure::PathTree>& t){
        return t->is_phrase() && creg.category_is_defined(t);
    };
    // auto is_inflection = [&wreg](const std::shared_ptr<ieml::structure::PathTree>& t){
    //     return t->is_inflection() && wreg.inflection_is_defined(std::dynamic_pointer_cast<ieml::structure::InflectionPathNode>(t->getNode()));
    // };

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it)
        graph.add_node(it->first);
    for (auto it = wreg.auxiliaries_begin(); it != wreg.auxiliaries_end(); it++)
        graph.add_node(it->first);
    for (auto it = wreg.inflections_begin(); it != wreg.inflections_end(); it++)
        graph.add_node(it->first);
    for (auto it = wreg.junctions_begin(); it != wreg.junctions_end(); it++)
        graph.add_node(it->first);
    for (auto it = wreg.category_word_begin(); it != wreg.category_word_end(); it++) 
        graph.add_node(it->second);

    for (auto it = creg.categories_begin(); it != creg.categories_end(); ++it) {
        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(is_phrase, is_phrase)) {
            graph.add_relation((Relation){
                .source    = it->first,
                .target    = subphrase.second,
                .attribute = std::make_shared<CompositionAttribute>(
                    subphrase.first,
                    CompositionRelationType::COMPOSITION_PHRASE
                )
            });
        }

        // inflections
        for (auto& inflections : it->first->find_sub_tree(&ieml::structure::PathTree::is_inflection, is_phrase)) {
            for (auto& inflection: inflections.second->getNode()->getWords()) {
                graph.add_relation((Relation){
                    .source    = it->first,
                    .target    = inflection,
                    .attribute = std::make_shared<CompositionAttribute>(
                        inflections.first,
                        CompositionRelationType::COMPOSITION_INFLECTION
                    )
                });
            }
        }
        // auxiliaries
        for (auto& auxiliaries : it->first->find_sub_tree(&ieml::structure::PathTree::is_auxiliary, is_phrase)) {
            for (auto& auxiliary: auxiliaries.second->getNode()->getWords()) {
                graph.add_relation((Relation){
                    .source    = it->first,
                    .target    = auxiliary,
                    .attribute = std::make_shared<CompositionAttribute>(
                        auxiliaries.first,
                        CompositionRelationType::COMPOSITION_AUXILIARY
                    )
                });
            }
        }
        // junctions
        for (auto& junctions : it->first->find_sub_tree(&ieml::structure::PathTree::is_junction, is_phrase)) {
            for (auto& junction: junctions.second->getNode()->getWords()) {
                graph.add_relation((Relation){
                    .source    = it->first,
                    .target    = junction,
                    .attribute = std::make_shared<CompositionAttribute>(
                        junctions.first,
                        CompositionRelationType::COMPOSITION_JUNCTION
                    )
                });
            }
        }
        // words
        for (auto& words : it->first->find_sub_tree(&ieml::structure::PathTree::is_word, is_phrase)) {
            for (auto& word: words.second->getNode()->getWords()) {
                graph.add_relation((Relation){
                    .source    = it->first,
                    .target    = word,
                    .attribute = std::make_shared<CompositionAttribute>(
                        words.first,
                        CompositionRelationType::COMPOSITION_WORD
                    )
                });
            }
        }

    }
};

