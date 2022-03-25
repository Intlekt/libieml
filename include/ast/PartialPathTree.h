#pragma once

#include <optional>
#include <vector>
#include <unordered_map>

#include "structure/path/PathTree.h"
#include "structure/reference/ReferenceSchemaRegister.h"
#include "ast/Variable.h"


namespace ieml::AST {


class PartialPathTree {
public:
// TODO: only variable ??? how ?
    typedef std::pair<ieml::structure::PathTree::Ptr, std::shared_ptr<Variable>> PositionedVariable;

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

    const std::unordered_set<std::string> getVariableNames() const {
        std::unordered_set<std::string> res;
        for (const auto& r: references_)
            res.insert(r.second->getName());
        return res;
    }

    std::optional<ieml::structure::ReferenceSchema::Arguments> getReferenceSchemaArguments() const {
        // construire le reference schema
        // must be a singular sequence

        ieml::structure::PathTree::Ptr path_tree = singular_sequences_[0];

        std::unordered_multimap<std::string, ieml::structure::PathTree::Ptr> arguments_paths;
        std::unordered_map<std::string, ieml::structure::ReferenceType> references_type;
        std::unordered_set<std::string> argument_names;

        for (const auto& r : references_) {
                    // only variable at the moment ??
            const auto var_name = r.second->getName();
            if (references_type.find(var_name) == references_type.end()) {
                references_type.insert({var_name, ieml::structure::ReferenceType::PATH_TREE});   
            }
            arguments_paths.insert({var_name, r.first});
            argument_names.insert(var_name);
        }

        std::vector<ieml::structure::ReferenceType> references_type_res;
        std::vector<std::vector<ieml::structure::PathTree::Ptr>> arguments_paths_res;
        std::vector<std::string>    argument_names_res;

        for (const auto& a : argument_names) {
            argument_names_res.push_back(a);
            references_type_res.push_back(references_type.find(a)->second);

            const auto range = arguments_paths.equal_range(a);
            std::vector<ieml::structure::PathTree::Ptr> paths;
            for (auto it = range.first; it != range.second; it++) {
                paths.push_back(it->second);
            }
            arguments_paths_res.push_back(paths);
        }

        // ieml::structure::ReferenceSchema::Arguments
        const auto res = std::make_tuple(
            path_tree,
            // references_type_res,
            arguments_paths_res,
            argument_names_res
        );
        return res;
    }

private:
    const ieml::structure::PathTree::Vector singular_sequences_;
    const std::vector<PositionedVariable> references_;
};




}