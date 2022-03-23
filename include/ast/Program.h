#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IDeclaration.h"
#include "ParserContextManager.h"


namespace ieml::AST {
class Program: virtual public AST {
public:
    Program(CharRange::Ptr&& char_range,
            std::vector<std::shared_ptr<IDeclaration>>&& declarations) : 
        AST(std::move(char_range)), 
        declarations_(std::move(declarations)) {}

    std::string to_string() const override;

    void check_program(ieml::parser::ParserContextManager& ctx) {
        for (auto&& declaration: declarations_)
            declaration->check_declaration(ctx);
    }

private:
    std::vector<std::shared_ptr<IDeclaration>> declarations_;
};

}