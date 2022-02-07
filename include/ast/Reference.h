#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/PartialPathTree.h"

#include "ast/Variable.h"
#include "ast/Constants.h"


namespace ieml {
namespace AST {

class Reference : virtual public AST {
public:
    Reference(std::shared_ptr<CharRange>&& char_range,
              std::shared_ptr<Variable>&& variable) : 
        AST(std::move(char_range)),
        variable_(std::move(variable)) {};

    std::string to_string() const override {
        return "<" + variable_->to_string() + ">";
    }

    PartialPathTree::PositionedVariable check_reference(__attribute__((unused)) ieml::parser::ParserContextManager& ctx) const {
        return {nullptr, variable_};
    }

private:
    std::shared_ptr<Variable> variable_;
};

}}