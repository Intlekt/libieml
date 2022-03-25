#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IAuxiliary.h"
#include "ast/Identifier.h"
#include "ast/Word.h"
#include "structure/Word.h"


namespace ieml::AST {

class Auxiliary: virtual public AST, public IAuxiliary {
public:
    Auxiliary(CharRange::Ptr&& char_range,
              std::shared_ptr<Identifier>&& identifier) : 
        AST(std::move(char_range)),
        identifier_(std::move(identifier)) {}

    virtual std::string to_string() const override {
        return "*" + identifier_->to_string();
    }

    virtual structure::PathNode::Vector check_auxiliary(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
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

class WordAuxiliary: virtual public AST, public IAuxiliary {
public:
    WordAuxiliary(CharRange::Ptr&& char_range,
                  std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        word_(std::move(word)) {}

    virtual std::string to_string() const override {
        return "*" + word_->to_string();
    }

    virtual structure::PathNode::Vector check_auxiliary(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        const auto word = word_->check_is_defined(ctx, structure::WordType::AUXILIARY);
        if (!word) return {nullptr};
        const auto& auxiliary = std::dynamic_pointer_cast<structure::AuxiliaryWord>(word);

        if (!auxiliary->accept_role(role_type)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Invalid auxiliary for this role, '" + word_->getScript() + "' is declared for role '" + 
                    auxiliary->getRoleType()._to_string() + "', not '" + role_type._to_string() + "'."
            );
            return {nullptr};
        }

        return {std::make_shared<structure::AuxiliaryPathNode>(auxiliary)};
    }

private:
    std::shared_ptr<Word> word_;
};

}