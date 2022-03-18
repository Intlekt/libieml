#include "structure/ParadigmRegister.h"

#include <map>

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

typedef std::vector<size_t> Coordinate;

struct OrderCoordinate {
    size_t operator()(const Coordinate& a, const Coordinate& b) const {
        for (size_t i = 0; i < a.size(); i ++) {
            if (a[i] == b[i]) continue;
            if (a[i] < b[i]) return -1;
            return 1;
        }

        return 0;
    }

};


PathTree::TablePtr ParadigmRegister::buildFromPathTree(ieml::structure::PathTree::Register& reg,
                                                        PathTree::Ptr paradigm, 
                                                        std::vector<ieml::structure::PathTree::Set> dimension_paths) {
    

    Coordinate shape(dimension_paths.size());
    std::fill(shape.begin(), shape.end(), 0);

    // dimension -> {path_tree_set -> coordinate}
    // path_tree_set is the value of a singular sequence at dimension_paths[dimension]
    std::vector<std::unordered_map<PathTree::Set, size_t, hashKey, eqKey>> dim_path_to_coordinate(dimension_paths.size());

    std::unordered_map<std::shared_ptr<PathTree>, Coordinate> coordinates;
    std::map<Coordinate, std::shared_ptr<PathTree>, OrderCoordinate> path_trees;

    for (const auto& sseq: PathTree::singular_sequences(paradigm)) {
        Coordinate coordinate;

        for (size_t d = 0; d < dimension_paths.size(); ++d) {
            auto expanded = *reg.expand_path(sseq, dimension_paths[d]).begin();
            auto& dim_coordinates = dim_path_to_coordinate[d];

            auto it = dim_coordinates.find(expanded);

            size_t coord;
            if (it == dim_coordinates.end()) {
                dim_coordinates.insert({expanded, shape[d]});
                coord = shape[d];
                shape[d] += 1;
            } else 
                coord = it->second;
            
            coordinate.push_back(coord);
        }
        coordinates.insert({sseq, coordinate});
        path_trees.insert({coordinate, sseq});
    }
    
    auto invariant = reg.invariant_paths(paradigm);

    std::vector<std::vector<PathTree::Set>> dim_variations;
    for (auto& dim: dimension_paths)
        dim_variations.push_back(reg.expand_path(paradigm, dim));
    

    std::vector<std::vector<PathTree::Ptr>> headers;
    for (size_t d = 0; d < dimension_paths.size(); ++d) {
        PathTree::Set header_line;

        std::vector<PathTree::Set> invariant_dim;
        
        for (size_t i = 0; i < path_trees.size(); ++i)
            invariant_dim.push_back(invariant);

        // add the variable dimensions
        for (size_t d_v = 0; d_v < dimension_paths.size(); ++d_v) {
            if (d == d_v) continue;

            const auto variation = dim_variations[d_v];
            for (size_t i = 0; i < path_trees.size(); ++i)
                invariant_dim[i].insert(variation[i].begin(), variation[i].end());
        }


        // for this dimension, looks at all the ss in that dimension
        for (auto variation_dim : reg.expand_path(paradigm, dimension_paths[d])) {
            PathTree::Vector ss_header;
            
            // add invariant and current dimension invariant (the slice invariant in the cube)
            for (size_t i = 0; i < shape[d]; i++) {
                PathTree::Set ss_paths(invariant_dim[i].begin(), invariant_dim[i].end());

                // insert ss paths for this variation dim (current dimension variation)
                ss_paths.insert(variation_dim.begin(), variation_dim.end());

                // build the ss
                ss_header.push_back(reg.buildFromPaths(ss_paths));
            }
            
            // build the paradigm from the ss
            header_line.insert(reg.build_paradigm(ss_header));
        }

        // push the dimension of headers
        // TODO: I think that is wrong because it doesn't keep the order
        headers.push_back(PathTree::Vector(header_line.begin(), header_line.end()));
    }

    std::vector<PathTree::Ptr> values;
    for (auto& ss: path_trees)
        values.push_back(ss.second);

    return PathTree::TablePtr(TableNd<PathTree::Ptr>::create(values, shape, headers, paradigm));
}