#pragma once

#include "structure/link/function/WordCondition.h"
#include "structure/link/function/WordDomain.h"


namespace ieml::structure {

class WordFunction {
public:
    WordFunction(WordDomain&& domain, WordCondition::Ptr&& condition) :
        domain_(std::move(domain)),
        condition_(std::move(condition)) {}

    std::vector<WordCondition::VariableValuation> getValues(ScriptRegister& sreg, WordRegister& wreg) const {
        std::vector<WordCondition::VariableValuation> res;
        for (const auto& v : domain_.getValues(wreg)) {
            if (condition_->evaluate(sreg, v)) {
                res.push_back(v);
            }
        }
        return res;
    }


private:
    const WordDomain domain_;
    const WordCondition::Ptr condition_;
};

}