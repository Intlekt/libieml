#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "structure/link/Link.h"
#include "structure/link/function/WordDomain.h"

#include <optional>


namespace ieml::AST {

class IFunctionDomainList: virtual public AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(IFunctionDomainList)
    
    IFunctionDomainList(FunctionType function_type) : 
        function_type_(function_type) {};

    virtual std::optional<ieml::structure::WordDomain> check_domain(ieml::parser::ParserContextManager&, const ieml::structure::Link::Ptr&) const = 0;

    FunctionType getFunctionType() const {return function_type_;};

private:
    const FunctionType function_type_;
};

}