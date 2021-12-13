#pragma once

#include <vector>
#include <memory>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Reference.h"


namespace ieml {
namespace AST {

class IReferencable {
public:
    IReferencable(std::vector<std::shared_ptr<Reference>>&& references) : 
        references_(std::move(references)) {};

    std::string references_to_string() const {
        std::ostringstream os;

        // bool first = true;
        for (auto&& ref: references_) {
            // if (first) first = false;
            // else os << " ";
            os << " " << ref->to_string();
        }

        return os.str();
    }

private:
    std::vector<std::shared_ptr<Reference>> references_;

};

}
}