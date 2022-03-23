#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IFunctionDomainList.h"
#include "ast/Variable.h"
#include "ast/Word.h"

#include <vector>

namespace ieml::AST {


class WordFunctionDomain : public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordFunctionDomain)

    WordFunctionDomain(CharRange::Ptr&& char_range,
                       Variable::Ptr&& variable,
                       Word::Ptr&& paradigm) :
        AST(std::move(char_range)),
        variable_(std::move(variable)),
        paradigm_(std::move(paradigm)) {}

    virtual std::string to_string() const override {
        return variable_->to_string() + " in " + paradigm_->to_string();
    };

private:
    const Variable::Ptr variable_;
    const Word::Ptr paradigm_;

};


class WordFunctionDomainList : public virtual AST, public virtual IFunctionDomainList {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordFunctionDomainList)

    WordFunctionDomainList(CharRange::Ptr&& char_range,
                           std::vector<WordFunctionDomain::Ptr>&& domain_list) :
        AST(std::move(char_range)),
        IFunctionDomainList(FunctionType::WORD),
        domain_list_(std::move(domain_list)) {}
    
    virtual std::string to_string() const override {
        std::ostringstream os;
        os << "(";
        bool first = true;
        for (const auto& d: domain_list_) {
            if (first) first = false;
            else       os << ", ";
            os << d->to_string();
        }
        os << ")";
        return os.str();
    };

    virtual void check_domain(ieml::parser::ParserContextManager& ctx) const override {}

private:
    const std::vector<WordFunctionDomain::Ptr> domain_list_;
};



}