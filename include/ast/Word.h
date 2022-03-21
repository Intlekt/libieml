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

    virtual PartialPathTree::Optional check_category(parser::ParserContextManager& ctx) const override {
        const auto& word = check_category_word(ctx);
        if (!word) return {};

        return PartialPathTree({ctx.getPathTreeRegister().get_or_create(word)}, {});
    };

    virtual std::shared_ptr<structure::PathNode> check_category_word(parser::ParserContextManager& ctx) const {
        const auto word = check_is_defined(ctx, structure::WordType::CATEGORY);
        if (!word) return nullptr;
        return std::make_shared<structure::WordPathNode>(std::dynamic_pointer_cast<structure::CategoryWord>(word));
    };

    structure::Script::Ptr check_script(parser::ParserContextManager& ctx) const {
        const auto script = ctx.get_or_parse_script(word_str_);

        if (script == nullptr) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Invalid script literal \"" + word_str_ + "\""
            );
            return nullptr;
        }

        return script;
    };

    structure::Script::Ptr check_is_declared(parser::ParserContextManager& ctx, structure::WordType word_type) const {
        const auto script = check_script(ctx);
        if (!script)
            return nullptr;

        const auto& wregister = ctx.getWordRegister();
        if (!wregister.script_is_declared(script)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Script \"" + script->to_string() + "\" has not been declared."
            );
            return nullptr;
        }

        auto type_declared = wregister.get_script_type(script);
        if (type_declared != word_type) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Script \"" + script->to_string() + "\" has been declared with a different type, expected " + word_type._to_string() +
                ", got " + type_declared._to_string() + "."
            );
            return nullptr;
        }

        return script;
    };

    std::shared_ptr<structure::Word> check_is_defined(parser::ParserContextManager& ctx, structure::WordType word_type) const {
        const auto script = check_is_declared(ctx, word_type);
        if (!script)
            return nullptr;

        const auto& wregister = ctx.getWordRegister();
        const auto word = wregister.get_word_from_script(script);
        if (!word) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Word \"" + script->to_string() + "\" has not been defined."
            );
            return nullptr;
        }

        // if (word->getWordType() != +word_type) {
        //     ctx.getErrorManager().visitorError(
        //         getCharRange(),
        //         "Word \"" + script->to_string() + "\" has been defined with a different type, expected " + word_type._to_string() +
        //         ", got " + word->getWordType()._to_string() + "."
        //     );
        //     return nullptr;
        // }

        return word;
    };

    structure::Script::Ptr check_is_not_defined(parser::ParserContextManager& ctx, structure::WordType word_type) const {
        const auto script = check_is_declared(ctx, word_type);
        if (!script)
            return nullptr;

        const auto& wregister = ctx.getWordRegister();
        if (wregister.word_is_defined(script)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Word \"" + script->to_string() + "\" has already been defined."
            );
            return nullptr;
        }
        return script;
    }


    static std::shared_ptr<Word> createFromQuotedString(std::shared_ptr<CharRange>&& char_range, const std::string& s) {
        return std::make_unique<Word>(std::move(char_range), s.substr(1, s.size() - 2));
    }
private:
    const std::string word_str_;



};
}