#pragma once

#include <memory>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"
#include "ast/interfaces/IReferenceValue.h"


namespace ieml::AST {
class Identifier : virtual public AST, public ICategory, public IReferenceValue {
public:
    Identifier(std::unique_ptr<CharRange>&& char_range, 
               std::string name) : 
        AST(std::move(char_range)),
        ICategory(),
        IReferenceValue(), 
        name_(name) {};
    
    std::string to_string() const override {
        return name_;
    }; 

    const std::string& getName() const {
        return name_;
    }

private:
    const std::string name_;

};

}