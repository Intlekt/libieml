#pragma once

#include <string>

#include "AST.h"
#include "Constants.h"


namespace ieml {
namespace AST {

class Reference: public AST {



private:
    int identifier;
    ReferenceType reference_type;
    std::string value;
};

}}