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

    virtual std::shared_ptr<structure::PathTree> check_category(parser::ParserContext& ctx) const {
        auto word = ctx.resolve_word(word_str_);

        if (!word) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Word not defined in dictionary: " + word_str_
            );
            return nullptr;
        }

        return std::make_shared<structure::PathTree>(word);
    };


private:
    const std::string word_str_;
};
}