#pragma once


#include <vector>

#include "structure/path/PathTree.h"


namespace ieml::structure {

class ParadigmLayout {
public:
    static ParadigmLayout buildFromPathTree(
        PathTree::Register& reg,
        const std::shared_ptr<PathTree> path_tree,
        const std::vector<PathTree::Set> dimension_paths);

    typedef std::vector<size_t> Coordinate;

    std::vector<size_t> getShape() const {return shape_;}

    const Coordinate& getCoordinate(const std::shared_ptr<PathTree>& ss) const {
        const auto& it = positions_.find(ss);

        if (it == positions_.end())
            throw std::invalid_argument("Argument not a singular sequence of the paradigm of this layout.");

        return it->second;
    }; 

private:
    ParadigmLayout(const std::vector<size_t>& shape, 
                   const std::unordered_map<std::shared_ptr<PathTree>, Coordinate>& positions) :
        shape_(shape),
        positions_(positions) {}

    const std::vector<size_t> shape_;
    const std::unordered_map<std::shared_ptr<PathTree>, Coordinate> positions_;
};

}