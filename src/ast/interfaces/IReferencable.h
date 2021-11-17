#pragma once

#include <vector>
#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Reference.h"


namespace ieml {
namespace AST {

class IReferencable: virtual public AST {
public:
    IReferencable(std::vector<std::unique_ptr<Reference>>&& references) : 
        references_(std::move(references)) {};

private:
    std::vector<std::unique_ptr<Reference>> references_;

};

}
}