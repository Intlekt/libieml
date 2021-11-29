#pragma once

#include <sstream>
#include <string>
#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Word.h"
#include "ast/Constants.h"
#include "ast/Phrase.h"
#include "ParserContext.h"


namespace ieml::AST {


class Declaration: virtual public AST {
public:
    Declaration(DeclarationType declaration_type) : 
        declaration_type_(declaration_type) {};

    virtual void check_declaration(ieml::parser::ParserContext& ctx) = 0;

private:
    const DeclarationType declaration_type_;
};

class ComponentDeclaration: public Declaration, public ITranslatable {
public:
    ComponentDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         std::unique_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        ITranslatable(std::move(translations)),
        Declaration(DeclarationType::Component), 
        phrase_(std::move(phrase)) {};

    std::string to_string() const {
        return "@component " + translations_to_string() + " " + phrase_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto phrase = phrase_->check_phrase(ctx);
        auto name = check_translatable(ctx);

        if (!phrase || !name) {
            return;
        }
        
        ctx.define_category(name, phrase, false);
    };
private:
    std::unique_ptr<Phrase> phrase_;
};

class NodeDeclaration: public Declaration, public ITranslatable {
public:
    NodeDeclaration(std::unique_ptr<CharRange>&& char_range, 
                    std::vector<std::unique_ptr<LanguageString>>&& translations,
                    std::unique_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        ITranslatable(std::move(translations)),
        Declaration(DeclarationType::Node), 
        phrase_(std::move(phrase)) {};

    std::string to_string() const {
        return "@node " + translations_to_string() + " " + phrase_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto phrase = phrase_->check_phrase(ctx);
        auto name = check_translatable(ctx);

        if (!phrase || !name) {
            return;
        }
        
        ctx.define_category(name, phrase, true);
    };   

private:
    std::unique_ptr<Phrase> phrase_;
};


class WordDeclaration: public Declaration {
public:
    WordDeclaration(std::unique_ptr<CharRange>&& char_range, 
                    std::unique_ptr<Word>&& word) : 
        AST(std::move(char_range)),          
        Declaration(DeclarationType::Word), 
        word_(std::move(word)) {};


    std::string to_string() const {
        return "@word " + word_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto word = word_->check_word(ctx);

        if (!word) {
            return;
        }
        
        ctx.define_word(word);
    };

private:
    std::unique_ptr<Word> word_;
};


}
