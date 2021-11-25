#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"


namespace ieml::AST {
class Word: virtual public AST, public ICategory {
public:
    Word(std::unique_ptr<CharRange>&& char_range,
         std::string& word_str) :
      AST(std::move(char_range)),
      ICategory(),
      word_str_(word_str) {}

    virtual std::string to_string() const override {
        return "'" + word_str_ + "'";
    }

private:
    const std::string word_str_;
};
}