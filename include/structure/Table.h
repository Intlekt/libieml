#pragma once


#include "structure/path/PathTree.h"


namespace ieml::structure {
class Table {
public:
    typedef std::unordered_map<PathTree::Ptr, PathTree::Ptr> InvariantMapping;

    Table(PathTree::Ptr root,
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