#pragma once

#include "ast/interfaces/IDeclaration.h"
#include "ast/function/WordFunctionDomainList.h"
#include "ast/function/WordFunctionConditionExpr.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/interfaces/IFunctionDomainList.h"
#include "ast/function/WordFunctionConditionExpr.h"


namespace ieml::AST {



class FunctionDeclaration : public virtual AST, public virtual IDeclaration {
public:
    IEML_DECLARE_PTR_TYPE_AST(AST)

    FunctionDeclaration(CharRange::Ptr&& char_range,
                        FunctionType function_type,
                        Identifier::Ptr&& link,
                        IFunctionDomainList::Ptr&& domain,
                        WordFunctionConditionExpr::Ptr&& condition) : 
        AST(std::move(char_range)),
        IDeclaration(DeclarationType::FUNCTION),
        function_type_(function_type),
        link_(std::move(link)),
        domain_(std::move(domain)),
        condition_(std::move(condition)) {} 

    virtual std::string to_string() const override {
        return getDeclarationString() + " type:" + function_type_._to_string() + 
                                        " link:" + link_->to_string() + 
                                        " domain:" + domain_->to_string() +
                                        " condition:" + condition_->to_string() +
                                        " .";
    };

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

    virtual std::string getDeclarationString() const override {return "@function";};

private:
    const FunctionType function_type_;
    const Identifier::Ptr link_;
    const IFunctionDomainList::Ptr domain_;
    const WordFunctionConditionExpr::Ptr condition_;
};
}