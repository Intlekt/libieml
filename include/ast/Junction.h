#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"
#include "ast/Identifier.h"
#include "ast/Word.h"


namespace ieml::AST {

class Junction: virtual public AST, public IJunction {
public:
    Junction(CharRange::Ptr&& char_range,
              std::shared_ptr<Identifier>&& identifier) : 
        AST(std::move(char_range)),
        identifier_(std::move(identifier)) {}

    virtual std::string to_string() const override {
        return "&" + identifier_->to_string();
    }

    virtual std::shared_ptr<structure::JunctionWord> check_junction(parser::ParserContextManager& ctx) const override {
        auto junction_type = ctx.getWordRegister().resolve_junction(structure::LanguageString(ctx.getLanguage(), identifier_->getName()));

        if (junction_type == nullptr) {
            ctx.getErrorManager().visitorError(
                identifier_->getCharRange(),
                "Undefined junction identifer '" + identifier_->getName() + "'."
            );
            return nullptr;
        }

        return junction_type;
    }

private:
    std::shared_ptr<Identifier> identifier_;
};


class WordJunction: virtual public AST, public IJunction {
public:
    WordJunction(CharRange::Ptr&& char_range,
                 std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        word_(std::move(word)) {}

    virtual std::string to_string() const override {
        return "&" + word_->to_string();
    }

    virtual std::shared_ptr<structure::JunctionWord> check_junction(parser::ParserContextManager& ctx) const override {
        const auto word = word_->check_is_defined(ctx, structure::WordType::JUNCTION);
        if (!word) return nullptr;
        const auto& junction_type = std::dynamic_pointer_cast<structure::JunctionWord>(word);
        return junction_type;
    }

private:
    std::shared_ptr<Word> word_;
};

}