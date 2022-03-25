#pragma once

#include "structure/script/MultiplicativeScript.h"
#include "structure/script/ScriptRegister.h"
#include "structure/Word.h"
#include "ast/Constants.h"
#include "ast/macro_utils.h"

#include <unordered_map>
#include <string>
#include <vector>


namespace ieml::structure {

class WordCondition {
public:
    IEML_DECLARE_PTR_TYPE_STRUCTURE(WordCondition)

    typedef std::unordered_map<std::string, Word::Ptr> VariableValuation;

    virtual bool evaluate(ScriptRegister& sreg, const VariableValuation& valuation) const = 0;

    virtual std::unordered_set<std::string> getVariableNameSet() const = 0;
};


class BooleanWordCondition : public WordCondition {
public:
    BooleanWordCondition(const WordCondition::Ptr& left, const WordCondition::Ptr& right, AST::BooleanWordFunctionConditionExprOperatorType type) :
        left_(left),
        right_(right),
        type_(type) {}

    virtual bool evaluate(ScriptRegister& sreg, const VariableValuation& valuation) const override {
        const auto v_l = left_->evaluate(sreg, valuation);
        const auto v_r = right_->evaluate(sreg, valuation);

        switch (type_) {
        case +AST::BooleanWordFunctionConditionExprOperatorType::AND:
            return v_l && v_r;
        case +AST::BooleanWordFunctionConditionExprOperatorType::OR:
            return v_l || v_r;
        default:
            throw "INVALID boolean word condition";
        }
    };

    virtual std::unordered_set<std::string> getVariableNameSet() const override {
        auto l_res = left_->getVariableNameSet();
        const auto r_res = right_->getVariableNameSet();
        l_res.insert(r_res.begin(), r_res.end());
        return l_res;
    }

private:
    const WordCondition::Ptr left_;
    const WordCondition::Ptr right_;
    const AST::BooleanWordFunctionConditionExprOperatorType type_;

};


class WordAccessor {
public:
    WordAccessor(const std::string& variable_name,
                 size_t source_layer,
                 const std::vector<AST::WordAccessorType>& accessors) :
        variable_name_(variable_name),
        accessors_(accessors),
        source_layer_(source_layer),
        target_layer_(source_layer - accessors_.size()) {}

    Script::Ptr access(ScriptRegister& sreg, const WordCondition::VariableValuation& valuation) const {
        Script::Ptr s = valuation.find(variable_name_)->second->getScript();
    
        if (s->get_type() == +ScriptType::PRIMITIVE) {
            // source_layer_ == target_layer_ == 0
            return s;
        }
        if (s->get_type() != +ScriptType::MULTIPLICATION) {
            throw "Variable has to be a multiplicative script";
        }

        MultiplicativeScript::Ptr curr = dynamic_cast<MultiplicativeScript::Ptr>(s);

        for (auto a : accessors_) {
            Script::Ptr script;
            switch (a) {
            case +AST::WordAccessorType::SUBSTANCE:
                script = curr->getSubstance();
                break;
            case +AST::WordAccessorType::ATTRIBUTE:
                script = curr->getAttribute();
                break;
            case +AST::WordAccessorType::MODE:
                script = curr->getMode();
                break;
            default:
                throw "invalid word accessor type";
            }

            if (script->is_nullscript())
                return sreg.get_nullscript(target_layer_);

            if (script->get_type() == +ScriptType::PRIMITIVE)
                return script;

            curr = dynamic_cast<MultiplicativeScript::Ptr>(script);
        }

        return curr;
    }

    std::string getVariableName() const {
        return variable_name_;
    };


private:
    const std::string variable_name_;
    const std::vector<AST::WordAccessorType> accessors_;

    const size_t source_layer_;
    const size_t target_layer_;
};



class EqualWordCondition : public WordCondition {
public:
    EqualWordCondition(WordAccessor&& left, WordAccessor&& right) :
        left_(std::move(left)),
        right_(std::move(right)) {}

    virtual bool evaluate(ScriptRegister& sreg, const VariableValuation& valuation) const override {
        const auto v_l = left_.access(sreg, valuation);
        const auto v_r = right_.access(sreg, valuation);
        return v_l == v_r;
    };

    virtual std::unordered_set<std::string> getVariableNameSet() const override {
        return {left_.getVariableName(), right_.getVariableName()};
    };

private:
    const WordAccessor left_;
    const WordAccessor right_;

};

}