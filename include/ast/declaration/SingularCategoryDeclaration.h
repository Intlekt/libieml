#pragma once


#include "ast/declaration/CategoryDeclaration.h"


namespace ieml::AST {



class SingularCategoryDeclaration: public CategoryDeclaration {
public:
    SingularCategoryDeclaration(std::shared_ptr<CharRange>&& char_range, 
                                std::vector<std::shared_ptr<LanguageString>>&& translations,
                                std::shared_ptr<Phrase>&& phrase,
                                DeclarationType declaration_type) : 
        AST(std::move(char_range)), 
        CategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            declaration_type)
         {};


    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 std::shared_ptr<structure::PathTree>& phrase) const override {
        ctx.getCategoryRegister().define_category(name, phrase, 
            (getDeclarationType() == +DeclarationType::NODE ? 
                structure::DefinitionType::NODE : 
                structure::DefinitionType::COMPONENT)
        );
    }

protected:
    virtual std::shared_ptr<structure::PathTree> _check_phrase(ieml::parser::ParserContextManager& ctx) const override {
        auto phrase_set = getPhrase()->check_phrase(ctx);

        if (phrase_set.size() != 1) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Paradigms can only be defined with a @paranode declaration."
            );
            return nullptr;
        }

        return *phrase_set.begin();
    };

};


}