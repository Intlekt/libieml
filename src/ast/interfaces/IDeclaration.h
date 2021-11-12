#pragma once

#include "ast/Constants.h"


namespace ieml::AST {

class IDeclaration {
public:
    IDeclaration(DeclarationType declaration_type) : declaration_type_(declaration_type) {};


private:
    const DeclarationType declaration_type_;
    
};

}