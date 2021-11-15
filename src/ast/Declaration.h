#pragma once

#include <sstream>
#include <string>
#include <memory>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Constants.h"

namespace ieml::AST {


class Declaration: public AST, public IDeclaration, public ITranslatable {
public:
    Declaration(std::unique_ptr<CharRange>&& char_range, 
                std::vector<std::unique_ptr<LanguageString>>&& translations,
                DeclarationType declaration_type) : 
        AST(std::move(char_range)), 
        IDeclaration(declaration_type), 
        ITranslatable(std::move(translations)) {};

};

class ComponentDeclaration: public Declaration {
public:
    ComponentDeclaration(std::unique_ptr<CharRange>&& char_range, 
                         std::vector<std::unique_ptr<LanguageString>>&& translations) : 
        Declaration(std::move(char_range), 
        std::move(translations), 
        DeclarationType(Component)) {};

    std::string to_string() const {
        std::ostringstream os;
        os << "@component " << translations_to_string() << " " << " .";
        return os.str();
    };
};

}
