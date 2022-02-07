#pragma once

#include <vector>
#include <memory>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/PartialPathTree.h"
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
        for (auto&& ref: references_)
            os << " " << ref->to_string();
        
        return os.str();
    }

    PartialPathTree::Optional check_references(ieml::parser::ParserContextManager& ctx) const {
        std::vector<PartialPathTree::PositionedVariable> res;
        res.reserve(references_.size());
        for (const auto& ref: references_)
            res.emplace_back(ref->check_reference(ctx));

        return PartialPathTree({}, std::move(res));
    }

private:
    std::vector<std::shared_ptr<Reference>> references_;
};

}
}