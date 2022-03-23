#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/Identifier.h"


namespace ieml::AST {

class LanguageDeclaration: public virtual AST, public IDeclaration {
public:
    LanguageDeclaration(CharRange::Ptr&& char_range,
                        std::shared_ptr<Identifier>&& language_type) : 
        AST(std::move(char_range)),
        IDeclaration(DeclarationType::LANGUAGE),
        language_type_(std::move(language_type)) {}

    virtual std::string to_string() const override;

    virtual std::string getDeclarationString() const override {return "@language";};

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

private:
    std::shared_ptr<Identifier> language_type_;
};

}

