#pragma once

#include <string>

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"


namespace ieml {
namespace AST {

class Reference {
public:
    Reference(int identifier, ReferenceType reference_type, const std::string& value) : 
        identifier_(identifier), reference_type_(reference_type_), value_(value) {};

private:
    const int identifier_;
    const ReferenceType reference_type_;
    const std::string value_;
};

}}