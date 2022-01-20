#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "structure/path/PathNode.h"
#include "ParserContextManager.h"

#include "structure/Constants.h"

namespace ieml::AST {

class IInflectionList : virtual public AST {
public:
    virtual structure::PathNode::Set check_inflection_list(parser::ParserContextManager& ctx, structure::RoleType role_type) const = 0;
};

}