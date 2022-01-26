#pragma once

#include "structure/path/PathTree.h"


namespace ieml::structure {

class ParadigmRegister {
public:

    void define_paradigm(PathTree::Register& reg, const std::shared_ptr<PathTree>& paradigm) {
        auto invariant = PathTree::paradigm_invariant(reg, paradigm);
        
        paradigms_.insert({paradigm});
        invariant_to_paradigm_.insert({invariant, paradigm});
    }

    bool is_defined(const std::shared_ptr<PathTree>& paradigm) {
        return paradigms_.find(paradigm) != paradigms_.end();
    }

    /**
     * @brief Return the paradigm associated with the invariant if defined, else return nullptr.
     * 
     * @param node 
     * @return std::shared_ptr<PathTree> 
     */
    std::shared_ptr<PathTree> resolve_paradigm(const std::shared_ptr<PathTree>& node) const {
        const auto it = invariant_to_paradigm_.find(node);
        if (it == invariant_to_paradigm_.end())
            return nullptr;

        return it->second;
    }

private:

    /**
     * @brief A hash map that links singular path tree to theirs corresponding paradigm.
     */
    std::unordered_map<std::shared_ptr<PathTree>, std::shared_ptr<PathTree>> invariant_to_paradigm_;

    std::unordered_set<std::shared_ptr<PathTree>> paradigms_;

};

}