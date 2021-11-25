#pragma once

#include <sstream>
#include <string>
#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Constants.h"
#include "ast/Phrase.h"
#include "ParserContext.h"


namespace ieml::AST {


class Declaration: virtual public AST, public ITranslatable {
public:
    Declaration(std::vector<std::unique_ptr<LanguageString>>&& translations,
                DeclarationType declaration_type) : 
        declaration_type_(declaration_type), 
        ITranslatable(std::move(translations)) {};

    virtual void check_declaration(ieml::parser::ParserContext& ctx) = 0;
        
private:
    const DeclarationType declaration_type_;

};

class ComponentDeclaration: public Declaration {
public:
    ComponentDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations,
                         std::unique_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)),          
        Declaration(std::move(translations), DeclarationType(Component)), 
        phrase_(std::move(phrase)) {};

    std::string to_string() const {
        std::ostringstream os;
        os << "@component " << translations_to_string() << " " << phrase_->to_string() << " .";
        return os.str();
    };

    void check_declaration(ieml::parser::ParserContext& ctx) override {
        auto phrase = phrase_->check_phrase(ctx);
        auto name = check_translatable(ctx);

        if (!phrase || !name) {
            return;
        }
        
        ctx.getNamespace().define(name, phrase);
    };   

private:
    std::unique_ptr<Phrase> phrase_;
};

}
