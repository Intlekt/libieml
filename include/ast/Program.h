#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/Declaration.h"
#include "ParserContext.h"


namespace ieml::AST {
class Program: virtual public AST {
public:
    Program(std::shared_ptr<CharRange> char_range,
            std::vector<std::shared_ptr<Declaration>>&& declarations) : 
        AST(std::move(char_range)), 
        declarations_(std::move(declarations)) {}

    std::string to_string() const override;

    void check_program(ieml::parser::ParserContext& ctx) {
        for (auto&& declaration: declarations_)
            declaration->check_declaration(ctx);
    }

private:
    std::vector<std::shared_ptr<Declaration>> declarations_;
};

}