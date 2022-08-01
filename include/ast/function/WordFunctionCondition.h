#pragma once

#include "ast/interfaces/AST.h"
#include "ast/function/WordAccessor.h"

namespace ieml::AST
{

    /**
     * @brief Abstract class representing comparison function between two Word accessor, left and right.
     *        The two word accessor are assumed to yield same layer scripts.
     *        This class is subclassed for script binary operators (==, !=, etc...)
     */
    class WordFunctionCondition : public virtual AST
    {
    public:
        IEML_DECLARE_PTR_TYPE_AST(WordFunctionCondition)

        WordFunctionCondition(CharRange::Ptr &&char_range,
                              WordAccessor::Ptr &&left,
                              WordAccessor::Ptr &&right) : AST(std::move(char_range)),
                                                           left_(std::move(left)),
                                                           right_(std::move(right)) {}

        /**
         * @brief Instanciate a ieml::structure::WordCondition subclass corresponding to this WordFunctionCondition subclass.
         *
         * @param left
         * @param right
         * @return ieml::structure::WordCondition::Ptr
         */
        virtual ieml::structure::WordCondition::Ptr build_word_condition(
            ieml::structure::WordAccessor::Ptr &&left,
            ieml::structure::WordAccessor::Ptr &&right) const = 0;

        virtual std::string to_string_operator() const = 0;

        virtual std::string to_string() const override { return left_->to_string() + " " + to_string_operator() + " " + right_->to_string(); }

        std::optional<ieml::structure::EqualWordCondition::Ptr> check_word_condition(ieml::parser::ParserContextManager &ctx,
                                                                                     const ieml::structure::Link::Ptr &link,
                                                                                     const ieml::structure::WordDomain &domain) const
        {
            const auto left = left_->check_accessor(ctx, link, domain);
            const auto right = right_->check_accessor(ctx, link, domain);
            if (!left || !right)
                return {};

            size_t l_layer_comp = 0;
            size_t r_layer_comp = 0;

            ieml::structure::WordAccessor::Ptr l_accessor;
            ieml::structure::WordAccessor::Ptr r_accessor;

            if (left->type_ == WordAccessor::WordAccessorArgs::Type::LITERAL)
            {
                l_layer_comp = left->script_->get_layer();
                l_accessor = std::make_shared<ieml::structure::LiteralWordAccessor>(
                    left->script_);
            }
            else
            {
                const auto l_dom = domain.find(left->name_);
                if (l_dom == domain.end())
                {
                    ctx.getErrorManager().visitorError(
                        left_->getCharRange(),
                        "Variable not defined : " + left->name_ + ".");
                    return {};
                }

                const auto l_layer = l_dom->second->get_layer();
                if (l_layer < left->accessors_.size())
                {
                    ctx.getErrorManager().visitorError(
                        left_->getCharRange(),
                        "Left operand has a negative layer.");
                    return {};
                }

                l_layer_comp = l_layer - left->accessors_.size();

                l_accessor = std::make_shared<ieml::structure::VariableWordAccessor>(
                    left->name_,
                    l_layer,
                    left->accessors_);
            }

            if (right->type_ == WordAccessor::WordAccessorArgs::Type::LITERAL)
            {
                r_layer_comp = right->script_->get_layer();
                r_accessor = std::make_shared<ieml::structure::LiteralWordAccessor>(
                    right->script_);
            }
            else
            {
                const auto r_dom = domain.find(right->name_);
                if (r_dom == domain.end())
                {
                    ctx.getErrorManager().visitorError(
                        right_->getCharRange(),
                        "Variable not defined : " + right->name_ + ".");
                    return {};
                }

                const auto r_layer = r_dom->second->get_layer();
                if (r_layer < right->accessors_.size())
                {
                    ctx.getErrorManager().visitorError(
                        right_->getCharRange(),
                        "Right operand has a negative layer.");
                    return {};
                }
                r_layer_comp = r_layer - right->accessors_.size();

                r_accessor = std::make_shared<ieml::structure::VariableWordAccessor>(
                    right->name_,
                    r_layer,
                    right->accessors_);
            }

            if (l_layer_comp != r_layer_comp)
            {
                ctx.getErrorManager().visitorError(
                    right_->getCharRange(),
                    "Right operand has a different layer than the left operand.");
                return {};
            }

            return build_word_condition(
                std::move(l_accessor),
                std::move(r_accessor));
        }

    private:
        const WordAccessor::Ptr left_;
        const WordAccessor::Ptr right_;
    };

    class WordFunctionConditionEquality : public virtual AST, public WordFunctionCondition
    {
    public:
        IEML_DECLARE_PTR_TYPE_AST(WordFunctionConditionEquality)

        WordFunctionConditionEquality(CharRange::Ptr &&char_range,
                                      WordAccessor::Ptr &&left,
                                      WordAccessor::Ptr &&right) : AST(std::move(char_range)),
                                                                   WordFunctionCondition(nullptr, std::move(left), std::move(right)) {}

        virtual ieml::structure::WordCondition::Ptr build_word_condition(
            ieml::structure::WordAccessor::Ptr &&left,
            ieml::structure::WordAccessor::Ptr &&right) const override
        {
            return std::make_shared<ieml::structure::EqualWordCondition>(
                std::move(left),
                std::move(right));
        }

        virtual std::string to_string_operator() const override { return "=="; }
    };

    class WordFunctionConditionInequality : public virtual AST, public WordFunctionCondition
    {
    public:
        IEML_DECLARE_PTR_TYPE_AST(WordFunctionConditionInequality)

        WordFunctionConditionInequality(CharRange::Ptr &&char_range,
                                        WordAccessor::Ptr &&left,
                                        WordAccessor::Ptr &&right) : AST(std::move(char_range)),
                                                                     WordFunctionCondition(nullptr, std::move(left), std::move(right)) {}

        virtual ieml::structure::WordCondition::Ptr build_word_condition(
            ieml::structure::WordAccessor::Ptr &&left,
            ieml::structure::WordAccessor::Ptr &&right) const override
        {
            return std::make_shared<ieml::structure::NotEqualWordCondition>(
                std::move(left),
                std::move(right));
        }

        virtual std::string to_string_operator() const override { return "!="; }
    };

}