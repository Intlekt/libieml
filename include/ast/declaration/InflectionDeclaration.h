#pragma once


#include "ast/declaration/ToolWordDeclaration.h"


namespace ieml::AST {

class InflectionDeclaration: public virtual AST, public ToolWordDeclaration {
public:
    InflectionDeclaration(std::shared_ptr<CharRange>&& char_range, 
                          std::vector<std::shared_ptr<LanguageString>>&& translations,
                          std::shared_ptr<Identifier>&& type,
                          std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::INFLECTION),
        type_(std::move(type)) {}
    
    virtual std::string to_string() const override;

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;
    
    virtual std::string getDeclarationString() const override {return "@inflection";};

private:
    const std::shared_ptr<Identifier> type_;
};

}