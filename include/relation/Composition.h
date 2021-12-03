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

    std::shared_ptr<structure::Path> getPath() const {return path_;};
private:
    const std::shared_ptr<structure::Path> path_;
};


class CompositionRelationGraph : public BinaryRelationGraph<structure::PathTree, CompositionRelation> {
public:

    static std::shared_ptr<CompositionRelationGraph> buildFromCategoryRegister(const structure::CategoryRegister& reg) {
        auto graph = std::make_shared<CompositionRelationGraph>();

        for (auto it = reg.categories_begin(); it != reg.categories_end(); ++it) {
            // for all subphrase in phrase
            for (auto& subphrase : it->first->find_sub_tree(is_phrase)) {
                if (reg.category_is_defined(subphrase.second))
                    graph->add_relation(std::make_shared<CompositionRelation>(
                        it->first,
                        subphrase.second,
                        subphrase.first
                    ));
            }
        }

        return graph;
    }
private:
    static bool is_phrase(const std::shared_ptr<structure::PathTree>& t) {return t->is_phrase();};
};
}