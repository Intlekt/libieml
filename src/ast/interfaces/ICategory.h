#pragma once

#include <memory>

#include "ast/interfaces/AST.h"

namespace ieml {
namespace AST {

class ICategory : virtual public AST{
public:
    ICategory() {}
};

}
}