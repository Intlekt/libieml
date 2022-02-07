#include "ast/PartialPathTree.h"


using namespace ieml::AST;


PartialPathTree::Optional PartialPathTree::concat(std::vector<PartialPathTree::Optional>&& trees) {
    // size_t pt_vector_size = 1;
    // size_t var_vector_size = 1;

    // for (const auto& opt_pt: trees) {
    //     if (!opt_pt)
    //         continue;

    //     pt_vector_size += opt_pt->singular_sequences_.size();
    //     var_vector_size += opt_pt->references_.size();
    // }

    ieml::structure::PathTree::Vector pt_vector;
    std::vector<PositionedVariable>   var_vector;

    // if (pt_vector_size == 0 && var_vector_size == 0)
    //     return {};

    for (auto&& opt_pt: trees) {
        if (!opt_pt)
            continue;
        
        pt_vector.insert(pt_vector.end(), 
                        std::make_move_iterator(opt_pt->singular_sequences_.begin()), 
                        std::make_move_iterator(opt_pt->singular_sequences_.end()));
        
        var_vector.insert(var_vector.end(),
                        std::make_move_iterator(opt_pt->references_.begin()), 
                        std::make_move_iterator(opt_pt->references_.end()));
    }

    return PartialPathTree(std::move(pt_vector), std::move(var_vector));
}

PartialPathTree::Optional PartialPathTree::product(ieml::structure::PathTree::Register& reg, 
                                                   const std::shared_ptr<ieml::structure::PathNode>& node, 
                                                   std::vector<PartialPathTree::Optional>&& trees) {
    std::vector<ieml::structure::PathTree::Vector> singular_sequences;
    singular_sequences.reserve(trees.size());

    for (const auto& tree: trees) {
        if (!tree)
            continue;
        singular_sequences.push_back(tree->getPathTrees());
    }

    std::vector<PositionedVariable> var_vector;
    for (auto&& opt_pt: trees) {
        if (!opt_pt)
            continue;

        for (auto && opt: opt_pt->getReferences())
            var_vector.push_back(PositionedVariable{reg.get_or_create(node, {opt.first}), opt.second});
    }
    
    return PartialPathTree(reg.get_or_create_product(node, singular_sequences), 
                           std::move(var_vector));
}

PartialPathTree::Optional PartialPathTree::prepend_node(ieml::structure::PathTree::Register& reg, std::shared_ptr<ieml::structure::PathNode> node) const {
    ieml::structure::PathTree::Vector path_trees;
    path_trees.reserve(singular_sequences_.size());
    std::vector<PositionedVariable>   pos_vars;
    pos_vars.reserve(references_.size());
    
    for (const auto& pt: singular_sequences_)
        path_trees.push_back(reg.get_or_create(node, {pt}));

    for (const auto& ref: references_) {
        ieml::structure::PathTree::Set children;
        if (ref.first)
            children = {ref.first};
        pos_vars.push_back(PositionedVariable{reg.get_or_create(node, children), ref.second});
    }

    return PartialPathTree(std::move(path_trees), std::move(pos_vars));
}

PartialPathTree::Optional PartialPathTree::prepend_node_product(ieml::structure::PathTree::Register& reg, ieml::structure::PathNode::Vector nodes) const {
    if (nodes.size() == 1)
        return prepend_node(reg, nodes[0]);
    
    ieml::structure::PathTree::Vector path_trees;

    if (references_.size() != 0)
        throw std::invalid_argument("Cannot have a reference in a paradigm");
    
    for (const auto& pt: singular_sequences_)
        for (const auto& node: nodes)
            path_trees.push_back(reg.get_or_create(node, {pt}));


    return PartialPathTree(std::move(path_trees), {});
}
