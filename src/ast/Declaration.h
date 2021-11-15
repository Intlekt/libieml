#pragma once

#include <sstream>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Constants.h"

namespace ieml::AST {


class Declaration: public AST, public IDeclaration, public ITranslatable {
public:
    Declaration(CharRange char_range, 
                const TranslationsMap & translations,
                DeclarationType declaration_type) : 
        AST(char_range), 
        IDeclaration(declaration_type), 
        ITranslatable(translations) {};

};

class ComponentDeclaration: public Declaration {
public:
    ComponentDeclaration(CharRange char_range, 
                         const TranslationsMap & translations) : 
        Declaration(char_range, translations, DeclarationType(Component)) {};

    std::string to_string() const {
        std::ostringstream os;
        os << "@component " << translations_to_string() << " " << " .";
        return os.str();
    };
};

}
