#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ParserContextManager.h"


namespace ieml::AST {

class IDeclaration: virtual public AST {
public:
    IDeclaration(DeclarationType declaration_type) : 
        declaration_type_(declaration_type) {};

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const = 0;

    virtual std::string getDeclarationString() const = 0;
    DeclarationType getDeclarationType() const {return declaration_type_;};

private:
    const DeclarationType declaration_type_;
};

}