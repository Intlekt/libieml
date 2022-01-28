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
                                 std::shared_ptr<structure::PathTree>& phrase) const override;

protected:
    virtual std::shared_ptr<structure::PathTree> _check_phrase(ieml::parser::ParserContextManager& ctx) const override;

};

}