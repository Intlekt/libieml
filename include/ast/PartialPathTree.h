#pragma once


#include "structure/path/PathTree.h"
#include "ast/Variable.h"


namespace ieml::AST {


class PartialPathTree {
public:
    typedef std::pair<std::shared_ptr<ieml::structure::PathTree>, std::shared_ptr<Variable>> PositionedVariable;

    PartialPathTree(ieml::structure::PathTree::Vector&& singular_sequences,
                    std::vector<PositionedVariable>&& references) : 
        singular_sequences_(std::move(singular_sequences)),
        references_(std::move(references)) {}

    typedef std::optional<PartialPathTree> Optional;

    /**
     * @brief Merge OptionalPartialPathTree into a unique one
     * 
     * @param l0 
     * @param l1 
     * @return OptionalPartialPathTree 
     */    
    static Optional product(ieml::structure::PathTree::Register& reg, const std::shared_ptr<ieml::structure::PathNode>& node, std::vector<Optional>&& trees) ;

    static Optional concat(std::vector<Optional>&& trees);

    Optional prepend_node(ieml::structure::PathTree::Register& reg, std::shared_ptr<ieml::structure::PathNode>) const;

    Optional prepend_node_product(ieml::structure::PathTree::Register& reg, ieml::structure::PathNode::Vector) const;

    const ieml::structure::PathTree::Vector& getPathTrees() const {return singular_sequences_;}
    const std::vector<PositionedVariable>& getReferences() const {return references_;}

private:
    const ieml::structure::PathTree::Vector singular_sequences_;
    const std::vector<PositionedVariable> references_;
};




}