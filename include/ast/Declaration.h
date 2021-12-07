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

class CategoryDeclaration: public Declaration, public ITranslatable {
public:
    CategoryDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         std::unique_ptr<Phrase>&& phrase,
                         DeclarationType declaration_type) : 
        AST(std::move(char_range)), 
        ITranslatable(std::move(translations)),
        Declaration(declaration_type), 
        phrase_(std::move(phrase)) {};

    virtual bool isNode() const = 0;

    std::string to_string() const {
        return std::string(isNode() ? "node" : "component") + " " + translations_to_string() + " " + phrase_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto phrase = phrase_->check_phrase(ctx);
        auto name = check_translatable(ctx);

        if (!phrase || !name) {
            return;
        }
        
        if (ctx.getCategoryRegister().category_is_defined(phrase)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine phrase " + phrase->to_string() + " as a category, it has already been defined before."
            );
            return;
        }

        ctx.getCategoryRegister().define_category(name, phrase, isNode());
        
    };

private:
    std::unique_ptr<Phrase> phrase_;

};


class ComponentDeclaration: public CategoryDeclaration {
public:
    ComponentDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         std::unique_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        CategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            DeclarationType::Component) {};

    virtual bool isNode() const {return false;};

};

class NodeDeclaration: public CategoryDeclaration {
public:
    NodeDeclaration(std::unique_ptr<CharRange>&& char_range, 
                    std::vector<std::unique_ptr<LanguageString>>&& translations,
                    std::unique_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        CategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            DeclarationType::Node) {};

    virtual bool isNode() const {return true;};
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
        
        if (ctx.word_is_defined(word)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine word " + word->to_string() + " as a category word, it has already been defined before."
            );
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
            return;
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
            return;
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

class LanguageDeclaration: public Declaration {
public:
    LanguageDeclaration(std::unique_ptr<CharRange>&& char_range,
                        std::unique_ptr<Identifier>&& language_type) : 
        AST(std::move(char_range)),
        Declaration(DeclarationType::Language),
        language_type_(std::move(language_type)) {}

    std::string to_string() const {
        return "@language " + language_type_->to_string() + " .";
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto language = ieml::structure::LanguageType::_from_string_nocase_nothrow(language_type_->getName().c_str());

        if (!language) {
            ctx.getErrorManager().visitorError(
                language_type_->getCharRange(), 
                "Invalid language identifier for language declaration, got: '" + language_type_->getName() + "'."
            );
            return;
        }
        auto is_set = ctx.setLanguage(*language);
        if (!is_set) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine language of the file. It has already been defined before."
            );
            return;
        }

        return;
    };
 


private:
    std::unique_ptr<Identifier> language_type_;
};


}

