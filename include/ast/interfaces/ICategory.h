#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "structure/Path.h"
#include "ParserContext.h"

namespace ieml {
namespace AST {

class ICategory : virtual public AST{
public:
    ICategory() {}

    virtual std::shared_ptr<structure::PathTree> check_category(parser::ParserContext& ctx) const = 0;
};

}
}