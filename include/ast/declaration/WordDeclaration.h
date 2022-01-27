#pragma once

#include "ast/interfaces/IDeclaration.h"


namespace ieml::AST {


class WordDeclaration: public IDeclaration {
public:
    WordDeclaration(std::shared_ptr<CharRange>&& char_range, 
                    std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),          
        IDeclaration(DeclarationType::WORD), 
        word_(std::move(word)) {};


    std::string to_string() const {
        return getDeclarationString() + " " + word_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContextManager& ctx) override {
        auto word = word_->check_word(ctx);

        if (!word) {
            return;
        }

        auto& wregister = ctx.getWordRegister();

        if (wregister.word_is_defined(word)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine word " + word->to_string() + " as a category word, it has already been defined before."
            );
            return;
        }

        ctx.getSourceMapping().register_mapping(word, this);
        wregister.define_word(word);
    };

    virtual std::string getDeclarationString() const override {return "@word";};

private:
    std::shared_ptr<Word> word_;
};




}