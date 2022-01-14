#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "structure/Path.h"
#include "ParserContextManager.h"
#include "structure/Constants.h"


namespace ieml::AST {
class IAuxiliary : virtual public AST {
public:
    virtual structure::PathNode::Set check_auxiliary(parser::ParserContextManager& ctx, structure::RoleType role_type) const = 0;
};
}
