#pragma once

#include "ast/interfaces/AST.h"

#include "ast/InflexedCategory.h"


namespace ieml::AST {

class AuxiliarySubPhraseLine : public AST {
public:
    AuxiliarySubPhraseLine(std::unique_ptr<CharRange>&& char_range,
                           std::unique_ptr<Identifier>&& auxiliary) : 
        AST(std::move(char_range)),
        auxiliary_(std::move(auxiliary)) {}

protected:
    std::string auxiliary_to_string() const {
        if (auxiliary_)
            return "*" + auxiliary_->to_string() + " ";
        else
            return "";
    }

private:
    std::unique_ptr<Identifier> auxiliary_;
};

class SimpleAuxiliarySubPhraseLine : public AuxiliarySubPhraseLine {
public:
    SimpleAuxiliarySubPhraseLine(std::unique_ptr<CharRange>&& char_range,
                                 std::unique_ptr<Identifier>&& auxiliary,
                                 std::unique_ptr<InflexedCategory>&& flexed_category) : 
        AuxiliarySubPhraseLine(std::move(char_range), std::move(auxiliary)),
        flexed_category_(std::move(flexed_category)) {}


    std::string to_string() const override {
        return auxiliary_to_string() + flexed_category_->to_string();
    }


private:
    std::unique_ptr<InflexedCategory> flexed_category_;
};


}