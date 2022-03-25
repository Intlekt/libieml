#pragma once


#include "ast/interfaces/AST.h"
#include "ast/Identifier.h"


namespace ieml::AST {

class RoleType: public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(RoleType)

    RoleType(CharRange::Ptr&& char_range) : AST(std::move(char_range)) {}

    virtual std::optional<structure::RoleType> check_role_type(parser::ParserContextManager& ctx) const = 0;
};


class IntegerRoleType: public virtual AST, public RoleType {
public:
    IEML_DECLARE_PTR_TYPE_AST(IntegerRoleType)

    IntegerRoleType(CharRange::Ptr&& char_range,
                    size_t role_type) : 
        AST(std::move(char_range)),
        RoleType(nullptr),
        role_type_(role_type) {}

    virtual std::string to_string() const override {
        return std::to_string(role_type_);
    }

    virtual std::optional<structure::RoleType> check_role_type(parser::ParserContextManager& ctx) const override {
        auto type = structure::RoleType::_from_integral_nothrow(role_type_);

        if (!type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role number, got '" + std::to_string(role_type_) + "'"
            );
            return {};
        }

        return *type;
    }

private:
    const size_t role_type_;
};

class IdentifierRoleType: public virtual AST, public RoleType {
public:
    IEML_DECLARE_PTR_TYPE_AST(IdentifierRoleType)

    IdentifierRoleType(CharRange::Ptr&& char_range,
                       const std::shared_ptr<Identifier>&& role_type) : 
        AST(std::move(char_range)),
        RoleType(nullptr),
        role_type_(std::move(role_type)) {}

    virtual std::string to_string() const override {
        return role_type_->to_string();
    }

    virtual std::optional<structure::RoleType> check_role_type(parser::ParserContextManager& ctx) const override {
        auto type = structure::RoleType::_from_string_nothrow(role_type_->getName().c_str());

        if (!type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role identifier, got '" + role_type_->to_string() + "'"
            );
            return {};
        }

        return *type;
    }

private:
    const std::shared_ptr<Identifier> role_type_;
};

}