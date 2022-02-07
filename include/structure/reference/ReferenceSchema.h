#pragma once


#include <vector>


#include "structure/path/PathTree.h"
#include "structure/reference/ReferenceType.h"


namespace ieml::structure {
class ReferenceSchema {
public:
    /**
     * @brief An argument can be at several path in a path_tree
     * 
     */
    typedef std::pair<const std::vector<std::shared_ptr<PathTree>>, ReferenceType> Argument;

    ReferenceSchema(const std::vector<Argument>& arguments) : 
        arguments_(std::move(arguments)) {}


    static const ReferenceSchema Empty;


private:
    const std::vector<Argument> arguments_;
};


}