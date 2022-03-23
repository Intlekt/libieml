#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"


namespace ieml::AST {

class IFunctionDomainList: virtual public AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(IFunctionDomainList)
    
    IFunctionDomainList(FunctionType function_type) : 
        function_type_(function_type) {};

    virtual void check_domain(ieml::parser::ParserContextManager& ctx) const = 0;

    FunctionType getFunctionType() const {return function_type_;};

private:
    const FunctionType function_type_;
};

}