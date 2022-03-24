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

    std::optional<ieml::structure::EqualWordCondition::Ptr> check_word_condition(ieml::parser::ParserContextManager& ctx, 
                                                                                 const ieml::structure::Link::Ptr& link,
                                                                                 const ieml::structure::WordDomain& domain) const {
        const auto left = left_->check_accessor(ctx, link, domain);
        const auto right = right_->check_accessor(ctx, link, domain);
        if (!left || !right) return {};

        const auto l_dom = domain.find(left->first);
        if (l_dom == domain.end()) {
            ctx.getErrorManager().visitorError(
                left_->getCharRange(),
                "Variable not defined : " + left->first + "."
            );
            return {};
        }

        const auto l_layer = l_dom->second->get_layer();
        if (l_layer < left->second.size()) {
            ctx.getErrorManager().visitorError(
                left_->getCharRange(),
                "Left operand has a negative layer."
            );
            return {};
        }
        const auto l_layer_comp = l_layer - left->second.size();

        const auto r_dom = domain.find(right->first);
        if (r_dom == domain.end()) {
            ctx.getErrorManager().visitorError(
                right_->getCharRange(),
                "Variable not defined : " + right->first + "."
            );
            return {};
        }

        const auto r_layer = r_dom->second->get_layer();
        if (r_layer < right->second.size()) {
            ctx.getErrorManager().visitorError(
                right_->getCharRange(),
                "Right operand has a negative layer."
            );
            return {};
        }
        const auto r_layer_comp = r_layer - right->second.size();

        if (l_layer_comp != r_layer_comp) {
            ctx.getErrorManager().visitorError(
                right_->getCharRange(),
                "Right operand has a different layer than the left operand."
            );
            return {};
        }

        return std::make_shared<ieml::structure::EqualWordCondition>(
            ieml::structure::WordAccessor(
                left->first,
                l_layer,
                left->second
            ),
            ieml::structure::WordAccessor(
                right->first,
                r_layer,
                right->second
            )
        );
    }

private:
    const WordAccessor::Ptr left_;
    const WordAccessor::Ptr right_;
};

}