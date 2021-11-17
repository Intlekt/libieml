#pragma once

#include <memory>
#include <vector>
#include <sstream>
#include <string>

// #include "ast/interfaces/AST.h"
// #include "ast/Declaration.h"


namespace ieml::AST {
class Program: virtual public AST {
private:
    std::vector<std::unique_ptr<Declaration>> declarations_;


public:
    Program(std::unique_ptr<CharRange> char_range,
            std::vector<std::unique_ptr<Declaration>>&& declarations) : 
        AST(std::move(char_range)), 
        declarations_(std::move(declarations)) {}

    std::string to_string() const override {
        std::ostringstream os;
        for (auto && declaration: declarations_) {
            os << declaration->to_string() << std::endl;
        }
        return os.str();
        
    };


};

}