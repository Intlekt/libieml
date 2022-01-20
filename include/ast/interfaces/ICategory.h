#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "structure/path/PathTree.h"
#include "ParserContextManager.h"

namespace ieml {
namespace AST {

class ICategory : virtual public AST{
public:
    ICategory() {}

    virtual structure::PathTree::Set check_category(parser::ParserContextManager& ctx) const = 0;
};

}
}