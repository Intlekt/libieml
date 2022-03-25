#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "structure/path/PathNode.h"
#include "ParserContextManager.h"

#include "structure/Constants.h"

namespace ieml::AST {

class IInflectionList : virtual public AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(IInflectionList)

    virtual structure::PathNode::Vector check_inflection_list(parser::ParserContextManager& ctx, structure::RoleType role_type) const = 0;
};

}