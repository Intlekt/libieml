#pragma once

#include "ast/interfaces/AST.h"


namespace ieml::AST {
class Identifier : public AST {
public:
    Identifier(CharRange char_range, std::string name) : AST(char_range), name_(name) {};
    
    std::string to_string() const override {
        return name_;
    };

private:
    const std::string name_;

};

}