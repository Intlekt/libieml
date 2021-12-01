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

class ToolWordDeclaration: public Declaration, public ITranslatable {
public:
    ToolWordDeclaration(std::unique_ptr<CharRange>&& char_range, 
                        std::vector<std::unique_ptr<LanguageString>>&& translations,
                        std::unique_ptr<Word>&& word,
                        DeclarationType declaration_type) : 
        AST(std::move(char_range)),
        ITranslatable(std::move(translations)),
        Declaration(declaration_type), 
        word_(std::move(word)) {};

protected:
    std::unique_ptr<Word> word_;
};

class InflexingDeclaration: public ToolWordDeclaration {
public:
    InflexingDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         std::unique_ptr<Identifier>&& type,
                         std::unique_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::Inflexion),
        type_(std::move(type)) {}
    
    std::string to_string() const {
        return "@inflexing " + translations_to_string() + " " + type_->to_string() + " " + word_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto name = check_translatable(ctx);

        // test if type_ is either VERB or NOUN
        auto inflexing_type = structure::InflexingType::_from_string_nothrow(type_->getName().c_str());
        if (!inflexing_type) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Invalid identifier to specify the inflexing type, expected 'NOUN' or 'VERB', got: '" + type_->getName() + "'."
            );
        } 

        auto word = word_->check_word(ctx);

        if (!name | !word | !inflexing_type) {
            return;
        }
        
        auto inflexing_word = std::make_shared<structure::InflexingWord>(word->getScript(), *inflexing_type);

        if (ctx.word_is_defined(inflexing_word)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot redefine word " + word->to_string() + " as an inflexion, it has already been defined before."
            );
            return;
        }
        ctx.define_inflexing(name, inflexing_word);
    };

private:
    const std::unique_ptr<Identifier> type_;
};

class AuxiliaryDeclaration: public ToolWordDeclaration {
public:
    AuxiliaryDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         int accepted_role_type,
                         std::unique_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::Inflexion),
        accepted_role_type_(accepted_role_type) {}
    
    std::string to_string() const {
        return "@auxiliary " + translations_to_string() + " " + std::to_string(accepted_role_type_) + " " + word_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto name = check_translatable(ctx);

        auto role_type = structure::RoleType::_from_integral_nothrow(accepted_role_type_);
        if (!role_type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role number for auxiliary declaration, got '" + std::to_string(accepted_role_type_) + "'"
            );
        }

        auto word = word_->check_word(ctx);

        if (!name | !word | !role_type) {
            return;
        }
        
        auto auxiliary_word = std::make_shared<structure::AuxiliaryWord>(word->getScript(), *role_type);

        if (ctx.word_is_defined(auxiliary_word)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot redefine word " + word->to_string() + " as an auxiliary, it has already been defined before."
            );
            return;
        }
        ctx.define_auxiliary(name, auxiliary_word);
    };

private:
    const int accepted_role_type_;
};

class JunctionDeclaration: public ToolWordDeclaration {
public:
    JunctionDeclaration(std::unique_ptr<CharRange>&& char_range, 
                        std::vector<std::unique_ptr<LanguageString>>&& translations,
                        std::unique_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::Inflexion) {}
    
    std::string to_string() const {
        return "@junction " + translations_to_string() + " " + word_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto name = check_translatable(ctx);
        auto word = word_->check_word(ctx);

        if (!name | !word) {
            return;
        }
        
        auto junction_word = std::make_shared<structure::JunctionWord>(word->getScript());

        if (ctx.word_is_defined(junction_word)) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot redefine word " + word->to_string() + " as a junction, it has already been defined before."
            );
            return;
        }
        ctx.define_junction(name, junction_word);
    };
};


}

