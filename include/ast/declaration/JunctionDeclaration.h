#pragma once


#include "ast/declaration/ToolWordDeclaration.h"


namespace ieml::AST {

class JunctionDeclaration: public ToolWordDeclaration {
public:
    JunctionDeclaration(CharRange::Ptr&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::JUNCTION) {}
    
    virtual std::string to_string() const override;
    
    virtual std::string getDeclarationString() const override {return "@junction";};

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;
};

}