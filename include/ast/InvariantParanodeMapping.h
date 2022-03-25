#pragma once


#include "ast/interfaces/AST.h"
#include "ast/Identifier.h"


namespace ieml::AST {
class InvariantParanodeMapping : public virtual AST {
public:
    InvariantParanodeMapping(
        CharRange::Ptr&& char_range,
        std::shared_ptr<Identifier>&& invariant,
        std::shared_ptr<Identifier>&& paranode) :
            AST(std::move(char_range)),
            invariant_(std::move(invariant)),
            paranode_(std::move(paranode)) {}

    typedef std::optional<std::pair<ieml::structure::PathTree::Ptr, ieml::structure::PathTree::Ptr>> CheckResult;

    virtual std::string to_string() const override {return invariant_->to_string() + " -> " + paranode_->getName();}
    
    CheckResult check_mapping(ieml::parser::ParserContextManager& ctx) const;

private:
    const std::shared_ptr<Identifier> invariant_;
    const std::shared_ptr<Identifier> paranode_;

};
}