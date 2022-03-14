#pragma once


// Table definition represents a specific mapping between a subset of a paradigm singular sequences and other paradigms.
// A table definition is used to represent a hierarchical relation between table cells and table.

#include "structure/path/PathTree.h"



namespace ieml::structure {
class TableDefinition {
public:
    typedef std::unordered_map<PathTree::Ptr, PathTree::Ptr> InvariantMapping;

    typedef std::shared_ptr<TableDefinition> Ptr;

    TableDefinition(PathTree::Ptr root,
          InvariantMapping invariant_mapping) : 
        root_(root),
        invariant_mapping_(invariant_mapping) {}

    const InvariantMapping& getInvariantMapping() const {return invariant_mapping_;}
    const PathTree::Ptr& getRoot() const {return root_;}

private:
    const PathTree::Ptr root_;
    const InvariantMapping invariant_mapping_;
};
}