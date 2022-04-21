#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Identifier.h"
#include "ast/Variable.h"

#include "structure/link/function/WordCondition.h"

namespace ieml::AST {

class WordAccessor : public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordAccessor)

    WordAccessor(CharRange::Ptr&& char_range):
        AST(std::move(char_range)) {}


    struct WordAccessorArgs {
        enum Type {VARIABLE = 0, LITERAL = 1};

        WordAccessorArgs(structure::Script::Ptr script) : 
            type_(Type::LITERAL), 
            name_(),
            accessors_(),
            script_(script) {}
        
        WordAccessorArgs(const std::string& name, 
                         const std::vector<WordAccessorType>& accessors) : 
            type_(Type::VARIABLE), 
            name_(name),
            accessors_(accessors),
            script_(nullptr) {}


        const Type type_;
        const std::string name_;
        const std::vector<WordAccessorType> accessors_;
        const structure::Script::Ptr script_;
    };

    virtual std::optional<WordAccessorArgs> check_accessor(
        ieml::parser::ParserContextManager& ctx, 
        const ieml::structure::Link::Ptr& link,
        const ieml::structure::WordDomain& domain) const = 0;
};

class WordLiteralAccessor : public virtual AST, public virtual WordAccessor {
public:
    IEML_DECLARE_PTR_TYPE_AST(WordLiteralAccessor)

    WordLiteralAccessor(CharRange::Ptr&& char_range, Word::Ptr&& word):
        AST(std::move(char_range)), WordAccessor(nullptr), word_(std::move(word)) {}

    virtual std::string to_string() const override {return word_->to_string();}

    virtual std::optional<WordAccessorArgs> check_accessor(
        ieml::parser::ParserContextManager& ctx, 
        const ieml::structure::Link::Ptr&,
        const ieml::structure::WordDomain&) const override {
            const auto res = word_->check_script(ctx);
            if (!res) return {};
            return WordAccessorArgs(res);
        };

    private:
    const Word::Ptr word_;
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
    virtual std::optional<WordAccessorArgs> check_accessor(
        ieml::parser::ParserContextManager& ctx, 
        const ieml::structure::Link::Ptr& link,
        const ieml::structure::WordDomain& domain) const override {
        
        const auto res = accessor_->check_accessor(ctx, link, domain);
        if (!res) return {};

        auto res_pos = res->accessors_;
        res_pos.push_back(position_);
        
        return WordAccessorArgs(res->name_, res_pos);
    }

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

    virtual std::optional<WordAccessorArgs> check_accessor(
        ieml::parser::ParserContextManager&, 
        const ieml::structure::Link::Ptr&,
        const ieml::structure::WordDomain&) const override {

        return WordAccessorArgs(variable_->getName(), std::vector<WordAccessorType>{});

    }

private:
    const Variable::Ptr variable_;
};


}