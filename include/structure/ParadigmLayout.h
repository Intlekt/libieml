#pragma once


#include <vector>

#include "structure/path/PathTree.h"


namespace ieml::structure {

class ParadigmLayout {
public:
    static ParadigmLayout buildFromPathTree(
        ieml::structure::PathTree::Register& reg,
        const std::shared_ptr<ieml::structure::PathTree> path_tree,
        const std::vector<ieml::structure::PathTree::Set> dimension_paths);

    typedef std::vector<size_t> Coordinate;
private:
    ParadigmLayout(size_t nb_dimensions, const std::vector<Coordinate> positions) :
        nb_dimensions_(nb_dimensions),
        positions_(positions) {}

    const size_t nb_dimensions_;
    const std::vector<Coordinate> positions_;
};

}