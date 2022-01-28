#pragma once


#include "ast/declaration/CategoryDeclaration.h"
#include "ast/DimensionDefinition.h"


namespace ieml::AST {

class ParanodeDeclaration: public virtual AST, public CategoryDeclaration {
public:
    ParanodeDeclaration(std::shared_ptr<CharRange>&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::vector<std::shared_ptr<DimensionDefinition>>&& dimensions,
                        std::shared_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        CategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            DeclarationType::PARANODE),
        dimension_definitions_(std::move(dimensions)) {};

    virtual std::string getDeclarationString() const override {return "@paranode";};

    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 std::shared_ptr<structure::PathTree>& phrase) const override;
protected:
    virtual std::shared_ptr<structure::PathTree> _check_phrase(ieml::parser::ParserContextManager& ctx) const override;

    std::vector<structure::PathTree::Set> _check_dimension_definitions(ieml::parser::ParserContextManager& ctx) const;

private:
    const std::vector<std::shared_ptr<DimensionDefinition>> dimension_definitions_;

};

}