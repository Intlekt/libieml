#include "relation/BinaryRelation.h"
#include "relation/Composition.h"
#include "structure/Path.h"
#include "structure/CategoryRegister.h"


using namespace ieml::relation;


bool is_phrase(const std::shared_ptr<ieml::structure::PathTree>& t) {return t->is_phrase();};

template<>
std::shared_ptr<CompositionRelationGraph> CompositionRelationGraph::buildFromCategoryRegister(const ieml::structure::CategoryRegister& reg) {
    auto graph = std::make_shared<CompositionRelationGraph>();

    for (auto it = reg.categories_begin(); it != reg.categories_end(); ++it) {
        // for all subphrase in phrase
        for (auto& subphrase : it->first->find_sub_tree(is_phrase, false)) {
            if (reg.category_is_defined(subphrase.second))
                graph->add_relation(std::make_shared<CompositionRelation>(
                    it->first,
                    subphrase.second,
                    std::make_shared<CompositionRelationAttribute>(subphrase.first)
                ));
        }
    }

    return graph;
};

