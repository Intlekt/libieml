#include "structure/ParadigmLayout.h"


using namespace ieml::structure;


struct eqKey {
    bool operator()(const PathTree::Set& a, const PathTree::Set& b) const {
        return a == b;
    }
};

struct hashKey {
    size_t operator()(const PathTree::Set& a) const {
        size_t hash = 0;
        for (auto& pt: a) {
            hash_combine(hash, pt->hash());
        }
        return hash;
    }
};


ParadigmLayout ParadigmLayout::buildFromPathTree(
    ieml::structure::PathTree::Register& reg,
    const std::shared_ptr<ieml::structure::PathTree> paradigm, 
    const std::vector<ieml::structure::PathTree::Set> dimension_paths) {
    
    std::vector<size_t> curr_coords(dimension_paths.size());
    std::fill(curr_coords.begin(), curr_coords.end(), 0);

    std::vector<std::unordered_map<PathTree::Set, size_t, hashKey, eqKey>> dim_path_to_coordinate(dimension_paths.size());
    std::unordered_map<std::shared_ptr<PathTree>, Coordinate> coordinates;
    std::unordered_map<Coordinate, std::shared_ptr<PathTree>, HashCoordinate, EqCoordinate> path_trees;

    for (const auto& sseq: PathTree::singular_sequences(paradigm)) {
        Coordinate coordinate;

        for (size_t d = 0; d < dimension_paths.size(); ++d) {
            auto expanded = *reg.expand_path(sseq, dimension_paths[d]).begin();
            auto& dim_coordinates = dim_path_to_coordinate[d];

            auto it = dim_coordinates.find(expanded);

            size_t coord;
            if (it == dim_coordinates.end()) {
                dim_coordinates.insert({expanded, curr_coords[d]});
                coord = curr_coords[d];
                curr_coords[d] += 1;
            } else 
                coord = it->second;
            
            coordinate.push_back(coord);
        }
        coordinates.insert({sseq, coordinate});
        path_trees.insert({coordinate, sseq});
    }
    
    return ParadigmLayout(curr_coords, coordinates, path_trees);
}
