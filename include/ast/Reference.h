#pragma once

#include <string>
#include <memory>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IReferenceValue.h"

#include "ast/Identifier.h"
#include "ast/Constants.h"


namespace ieml {
namespace AST {

class Reference : virtual public AST {
public:
    Reference(std::shared_ptr<CharRange>&& char_range,
              std::shared_ptr<int>&& identifier, 
              std::shared_ptr<Identifier>&& reference_type, 
              std::shared_ptr<IReferenceValue>&& value) : 
        AST(std::move(char_range)),
        identifier_(std::move(identifier)), 
        reference_type_(std::move(reference_type)), 
        value_(std::move(value)) {};

    std::string to_string() const override {
        std::ostringstream os;
        os << "<";
        if (identifier_)
            os << "id " << *identifier_ << " "; 

        os << "dt " << reference_type_->getName() << " ";
        os << "va " << value_->to_string() << ">";

        return os.str();
    }

private:
    std::shared_ptr<int> identifier_;
    std::shared_ptr<Identifier> reference_type_;
    std::shared_ptr<IReferenceValue>value_;
};

}}