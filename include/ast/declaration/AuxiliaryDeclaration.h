#pragma once


#include "ast/declaration/ToolWordDeclaration.h"


namespace ieml::AST {

class AuxiliaryDeclaration: public virtual AST, public ToolWordDeclaration {
public:
    AuxiliaryDeclaration(std::shared_ptr<CharRange>&& char_range, 
                         std::vector<std::shared_ptr<LanguageString>>&& translations,
                         int accepted_role_type,
                         std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::AUXILIARY),
        accepted_role_type_(accepted_role_type) {}
    
    virtual std::string to_string() const override;

    virtual std::string getDeclarationString() const override {return "@auxiliary";};

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

private:
    const int accepted_role_type_;
};

}