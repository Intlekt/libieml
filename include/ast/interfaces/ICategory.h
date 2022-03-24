#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/PartialPathTree.h"
#include "structure/path/PathTree.h"
#include "ParserContextManager.h"

namespace ieml {
namespace AST {

class ICategory : virtual public AST{
public:
    IEML_DECLARE_PTR_TYPE_AST(ICategory)

    ICategory() {}

    virtual PartialPathTree::Optional check_category(parser::ParserContextManager& ctx) const = 0;
};

}
}