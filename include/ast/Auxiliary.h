#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IAuxiliary.h"
#include "ast/Identifier.h"


namespace ieml::AST {

class Auxiliary: virtual public AST, public IAuxiliary {
public:
    Auxiliary(std::shared_ptr<CharRange>&& char_range,
              std::shared_ptr<Identifier>&& identifier) : 
        AST(std::move(char_range)),
        identifier_(std::move(identifier)) {}

    virtual std::string to_string() const override {
        return "*" + identifier_->to_string();
    }

    virtual structure::PathNode::Set check_auxiliary(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        auto auxiliary = ctx.getWordRegister().resolve_auxiliary(structure::LanguageString(ctx.getLanguage(), identifier_->getName()));

        if (!auxiliary) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Undefined auxiliary identifier '" + identifier_->getName() + "'."
            );
            return {nullptr};
        }

        if (!auxiliary->accept_role(role_type)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Invalid auxiliary for this role, '" + identifier_->getName() + "' is declared for role '" + 
                    auxiliary->getRoleType()._to_string() + "', not '" + role_type._to_string() + "'."
            );
            return {nullptr};
        }

        return {std::make_shared<structure::AuxiliaryPathNode>(auxiliary)};
    }

private:
    std::shared_ptr<Identifier> identifier_;
};


}