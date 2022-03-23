#pragma once


#include <string>
#include <vector>


#include "structure/path/PathTree.h"
#include "structure/reference/ReferenceType.h"


namespace ieml::structure {
class ReferenceSchema {
public:
    /**
     * A Reference Schema is :
     *  - a path tree
     *  - a list of Reference type
     *  - a list of list of path in the Reference Schema path tree
     *  - a list of string representing argument name (the signature). If the argument name is empty string, 
     *      indicates that the argument is not free (not in the link )
     */

    ReferenceSchema(
        const PathTree::Ptr& path_tree,
        const std::vector<ReferenceType>& references_type,
        const std::vector<std::vector<PathTree::Ptr>>& arguments_paths,
        const std::vector<std::string>& argument_names
    ) : 
        path_tree_(path_tree),
        references_type_(references_type),
        arguments_paths_(arguments_paths),
        argument_names_(argument_names) {}

private:
    const PathTree::Ptr path_tree_;
    const std::vector<ReferenceType> references_type_;
    const std::vector<std::vector<PathTree::Ptr>> arguments_paths_;
    const std::vector<std::string> argument_names_;
};


}