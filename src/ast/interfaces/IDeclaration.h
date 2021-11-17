#pragma once

#include <memory>

#include "ast/Constants.h"
#include "ast/interfaces/AST.h"


namespace ieml::AST {

class IDeclaration : virtual public AST {
public:
    IDeclaration(DeclarationType declaration_type) : 
        declaration_type_(declaration_type) {};


private:
    const DeclarationType declaration_type_;
    
};

}