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

    std::shared_ptr<PathTree> at(const Coordinate& coord) const {
        const auto& it = path_trees_.find(coord);

        if (it == path_trees_.end()) 
            throw std::invalid_argument("Argument not a valid coordinate int this layout.");

        return it->second;
    } 

private:
    struct HashCoordinate {
        size_t operator()(const Coordinate& c0) const {
            size_t hash = 0;
            for (size_t c : c0)
                hash_combine(hash, c);
            return hash;
        }
    };

    struct EqCoordinate {
        bool operator()(const Coordinate& c0, const Coordinate& c1) const {
            return c0 == c1;
        }
    };

    ParadigmLayout(const std::vector<size_t>& shape, 
                   const std::unordered_map<std::shared_ptr<PathTree>, Coordinate>& positions,
                   const std::unordered_map<Coordinate, std::shared_ptr<PathTree>, HashCoordinate, EqCoordinate>& path_trees) :
        shape_(shape),
        positions_(positions),
        path_trees_(path_trees) {}


    const std::vector<size_t> shape_;
    const std::unordered_map<std::shared_ptr<PathTree>, Coordinate> positions_;
    const std::unordered_map<Coordinate, std::shared_ptr<PathTree>, HashCoordinate, EqCoordinate> path_trees_;
};

}