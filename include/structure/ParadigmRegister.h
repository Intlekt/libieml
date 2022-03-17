#pragma once


#include "structure/path/PathTree.h"
#include "structure/TableDefinition.h"
#include "structure/table/Table.h"


namespace ieml::structure {

class ParadigmRegister {
public:
    ParadigmRegister() = default;

    ParadigmRegister(const ParadigmRegister&) = delete;
    ParadigmRegister& operator=(ParadigmRegister&) = delete;

    void define_paradigm(PathTree::Register& reg, const std::shared_ptr<PathTree>& paradigm, const std::vector<ieml::structure::PathTree::Set> dimension_paths) {
        auto invariant = reg.buildFromPaths(reg.invariant_paths(paradigm));
        
        paradigms_.insert({paradigm, buildFromPathTree(reg, paradigm, dimension_paths)});
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
    PathTree::Set resolve_paradigms(const std::shared_ptr<PathTree>& node) const {
        auto it = invariant_to_paradigm_.find(node);
        
        PathTree::Set res;
        while(it != invariant_to_paradigm_.end()) {
            res.insert(it->second);
            ++it;
        }

        return res;
    }

    const PathTree::TablePtr& get_table(const std::shared_ptr<PathTree>& paradigm) const {
        return paradigms_.find(paradigm)->second;
    }

    const std::unordered_map<PathTree::Ptr, PathTree::TablePtr>& get_tables() const {
        return paradigms_;
    }


    void register_table(const TableDefinition::Ptr& table) {
        tables_.push_back(table);
    }

    const std::vector<TableDefinition::Ptr>& getTableHierarchies() const {
        return tables_;
    }

private:
    /**
     * @brief A hash map that links singular path tree to theirs corresponding paradigm.
     */
    std::unordered_multimap<PathTree::Ptr, PathTree::Ptr> invariant_to_paradigm_;

    std::unordered_map<PathTree::Ptr, PathTree::TablePtr> paradigms_;

    std::vector<TableDefinition::Ptr> tables_;

    static PathTree::TablePtr buildFromPathTree(PathTree::Register& reg,
                                                PathTree::Ptr paradigm, 
                                                std::vector<PathTree::Set> dimension_paths);

};

}