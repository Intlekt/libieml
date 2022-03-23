#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Identifier.h"
#include "ast/Variable.h"

namespace ieml::AST {

class WordAccessor : public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordAccessor)

    WordAccessor(CharRange::Ptr&& char_range):
        AST(std::move(char_range)) {}

private:

};


class MultiplicativeWordAccessor : public virtual AST, public WordAccessor {
public:

    IEML_DECLARE_PTR_TYPE_AST(MultiplicativeWordAccessor)

    MultiplicativeWordAccessor(CharRange::Ptr&& char_range,
                               WordAccessor::Ptr&& accessor,
                               WordAccessorType position):
        AST(std::move(char_range)),
        WordAccessor(nullptr),
        accessor_(std::move(accessor)),
        position_(position) {}

    virtual std::string to_string() const override {return accessor_->to_string() + "." + std::string(position_._to_string());}

private:
    const WordAccessor::Ptr accessor_;
    const WordAccessorType position_;
};


class VariableWordAccessor : public virtual AST, public WordAccessor {
public:

    IEML_DECLARE_PTR_TYPE_AST(VariableWordAccessor)

    VariableWordAccessor(CharRange::Ptr&& char_range,
                               Variable::Ptr&& variable):
        AST(std::move(char_range)),
        WordAccessor(nullptr),
        variable_(std::move(variable)) {}

    virtual std::string to_string() const override {return variable_->to_string();}

private:
    const Variable::Ptr variable_;
};


}