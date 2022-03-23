#pragma once

#include "ast/interfaces/AST.h"
#include "ast/function/WordAccessor.h"


namespace ieml::AST {

class WordFunctionCondition : public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordFunctionCondition)

    WordFunctionCondition(CharRange::Ptr&& char_range,
                          WordAccessor::Ptr&& left,
                          WordAccessor::Ptr&& right) :
        AST(std::move(char_range)),
        left_(std::move(left)),
        right_(std::move(right)) {}

    virtual std::string to_string() const override {return left_->to_string() + " == " + right_->to_string();}
private:
    const WordAccessor::Ptr left_;
    const WordAccessor::Ptr right_;
};

}