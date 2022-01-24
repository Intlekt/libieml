#pragma once

#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"


namespace ieml::AST {
class Word: virtual public AST, public ICategory {
public:
    Word(std::shared_ptr<CharRange>&& char_range,
         const std::string& word_str) :
      AST(std::move(char_range)),
      ICategory(),
      word_str_(word_str) {}

    std::string getScript() const {return word_str_;};

    virtual std::string to_string() const override {
        return "\"" + word_str_ + "\"";
    }

    virtual structure::PathTree::Set check_category(parser::ParserContextManager& ctx) const {
        const auto& word = check_category_word(ctx);
        if (!word) return {nullptr};

        return {ctx.getPathTreeRegister().get_or_create(word)};
    };

    virtual std::shared_ptr<structure::PathNode> check_category_word(parser::ParserContextManager& ctx) const {
        auto word = ctx.getWordRegister().resolve_category_word(word_str_);

        if (!word) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Word not defined in dictionary: " + word_str_
            );
            return nullptr;
        }

        return std::make_shared<structure::WordPathNode>(word);
    };

    virtual std::shared_ptr<structure::CategoryWord> check_word(__attribute__((unused)) parser::ParserContextManager& ctx) const {
        return std::make_shared<structure::CategoryWord>(word_str_);
    };

    static std::shared_ptr<Word> createFromQuotedString(std::shared_ptr<CharRange>&& char_range, const std::string& s) {
        return std::make_unique<Word>(std::move(char_range), s.substr(1, s.size() - 2));
    }
private:
    const std::string word_str_;
};
}