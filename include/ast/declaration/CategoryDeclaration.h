#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Phrase.h"


namespace ieml::AST {

class CategoryDeclaration: public virtual AST, public IDeclaration, public ITranslatable {
public:
    CategoryDeclaration(std::shared_ptr<CharRange>&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::shared_ptr<Phrase>&& phrase,
                        DeclarationType declaration_type) : 
        AST(std::move(char_range)), 
        IDeclaration(declaration_type),
        ITranslatable(std::move(translations)),
        phrase_(std::move(phrase)) {};

    std::string to_string() const override {
        return getDeclarationString() + " " + translations_to_string() + " " + phrase_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContextManager& ctx) override {
        auto phrase = _check_phrase(ctx);
        auto name = check_translatable(ctx);

        if (!phrase || !name) {
            return;
        }
        bool valid = true;
        if (ctx.getCategoryRegister().category_is_defined(phrase)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine phrase " + phrase->to_string() + ", it has already been defined before."
            );
            valid = false;
        }
        
        for (const auto& ls: *name) {
            if (ctx.getCategoryRegister().resolve_category(ls.second)) {
                ctx.getErrorManager().visitorError(
                    getCharRange(), 
                    "Cannot redefine language string '" + std::string(ls.second.language()._to_string()) +
                    ":" + ls.second.value() + "' with another category, it has already been defined before."
                );

                valid = false;
            }
        }

        if (!valid)
            return;

        ctx.getSourceMapping().register_mapping(phrase, this);
        define_category(ctx, name, phrase);
    };

    std::shared_ptr<Phrase> getPhrase() const {return phrase_;};

protected:

    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 std::shared_ptr<structure::PathTree>& phrase) const = 0;
    virtual std::shared_ptr<structure::PathTree> _check_phrase(ieml::parser::ParserContextManager& ctx) const = 0;

private:
    const std::shared_ptr<Phrase> phrase_;
};


}