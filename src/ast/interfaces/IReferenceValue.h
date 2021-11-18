#pragma once

#include "ast/interfaces/AST.h"

namespace ieml::AST {
class IReferenceValue : virtual public AST {
public:
    IReferenceValue() {}
};

}