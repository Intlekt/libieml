#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"

#include <optional>

namespace ieml::AST {

class IFunctionCondition: virtual public AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(IFunctionCondition)
    
    IFunctionCondition(FunctionType function_type) : 
        function_type_(function_type) {};


    FunctionType getFunctionType() const {return function_type_;};

private:
    const FunctionType function_type_;
};

}