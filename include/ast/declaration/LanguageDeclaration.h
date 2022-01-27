#pragma once


#include "ast/interfaces/IDeclaration.h"


namespace ieml::AST {

class LanguageDeclaration: public IDeclaration {
public:
    LanguageDeclaration(std::shared_ptr<CharRange>&& char_range,
                        std::shared_ptr<Identifier>&& language_type) : 
        AST(std::move(char_range)),
        IDeclaration(DeclarationType::LANGUAGE),
        language_type_(std::move(language_type)) {}

    std::string to_string() const {
        return getDeclarationString() + " " + language_type_->to_string() + " .";
    };
    virtual std::string getDeclarationString() const override {return "@language";};

    void check_declaration(ieml::parser::ParserContextManager& ctx) override {
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
    std::shared_ptr<Identifier> language_type_;
};

}

