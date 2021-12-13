#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"


namespace ieml::AST {
class Word: virtual public AST, public ICategory {
public:
    Word(std::unique_ptr<CharRange>&& char_range,
         const std::string& word_str) :
      AST(std::move(char_range)),
      ICategory(),
      word_str_(word_str) {}

    virtual std::string to_string() const override {
        return "'" + word_str_ + "'";
    }

    virtual std::shared_ptr<structure::PathTree> check_category(parser::ParserContext& ctx) const {
        auto word = ctx.getWordRegister().resolve_category_word(word_str_);

        if (!word) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Word not defined in dictionary: " + word_str_
            );
            return nullptr;
        }

        return ctx.getPathTreeRegister().get_or_create(std::make_shared<structure::WordPathNode>(word));
    };

    virtual std::shared_ptr<structure::CategoryWord> check_word(parser::ParserContext& ctx) const {
        return std::make_shared<structure::CategoryWord>(word_str_);
    };

    static std::unique_ptr<Word> createFromQuotedString(std::unique_ptr<CharRange>&& char_range, const std::string& s) {
        return std::make_unique<Word>(std::move(char_range), s.substr(1, s.size() - 2));
    }
private:
    const std::string word_str_;
};
}