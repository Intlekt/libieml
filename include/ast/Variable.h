#pragma once


#include "ast/interfaces/AST.h"


namespace ieml::AST {

class Variable: public virtual AST {
public:
    typedef std::shared_ptr<Variable> Ptr;

    Variable(CharRange::Ptr&& char_range,
             const std::string& name) : 
        AST(std::move(char_range)),
        name_(name) {}

    virtual std::string to_string() const override {
        return name_;
    }

    std::string getName() const {return name_;}
private:
    const std::string name_;

};
}