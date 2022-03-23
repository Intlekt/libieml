#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IFunctionCondition.h"
#include "ast/function/WordFunctionCondition.h"
#include "ast/Constants.h"


namespace ieml::AST {
class WordFunctionConditionExpr : public virtual AST, public IFunctionCondition {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordFunctionConditionExpr)

    WordFunctionConditionExpr(CharRange::Ptr&& char_range) :
        AST(std::move(char_range)),
        IFunctionCondition(FunctionType::WORD) {}

};

class ParenthesisWordFunctionConditionExpr : public virtual AST, public WordFunctionConditionExpr {
public:
    IEML_DECLARE_PTR_TYPE_AST(ParenthesisWordFunctionConditionExpr)

    ParenthesisWordFunctionConditionExpr(CharRange::Ptr&& char_range,
                                     WordFunctionConditionExpr::Ptr&& content):
        AST(std::move(char_range)),
        WordFunctionConditionExpr(nullptr),
        content_(std::move(content)) {}

    virtual std::string to_string() const override {return "(" + content_->to_string() + ")";}
    virtual void check_condition(ieml::parser::ParserContextManager& ctx) const override {}

private:
    const WordFunctionConditionExpr::Ptr content_;
};


class PredicateWordFunctionConditionExpr : public virtual AST, public WordFunctionConditionExpr {
public:
    IEML_DECLARE_PTR_TYPE_AST(PredicateWordFunctionConditionExpr)

 
    PredicateWordFunctionConditionExpr(CharRange::Ptr&& char_range,
                                       WordFunctionCondition::Ptr&& predicate) :
        AST(std::move(char_range)),
        WordFunctionConditionExpr(nullptr),
        predicate_(std::move(predicate)) {}

    virtual std::string to_string() const override {return predicate_->to_string();}
    virtual void check_condition(ieml::parser::ParserContextManager& ctx) const override {}

private:
    const WordFunctionCondition::Ptr predicate_;
};


class BooleanWordFunctionConditionExpr : public virtual AST, public WordFunctionConditionExpr {
public:
    IEML_DECLARE_PTR_TYPE_AST(BooleanWordFunctionConditionExpr)


    BooleanWordFunctionConditionExpr(CharRange::Ptr&& char_range,
                                     WordFunctionConditionExpr::Ptr&& left,
                                     WordFunctionConditionExpr::Ptr&& right,
                                     BooleanWordFunctionConditionExprOperatorType operator_type) :
        AST(std::move(char_range)),
        WordFunctionConditionExpr(nullptr),
        left_(std::move(left)),
        right_(std::move(right)),
        operator_(operator_type) {}

    virtual std::string to_string() const override {return left_->to_string() + " " + std::string(operator_._to_string()) + " " + right_->to_string();}
    virtual void check_condition(ieml::parser::ParserContextManager& ctx) const override {}
private:
    const WordFunctionConditionExpr::Ptr left_;
    const WordFunctionConditionExpr::Ptr right_;
    const BooleanWordFunctionConditionExprOperatorType operator_;
};


}