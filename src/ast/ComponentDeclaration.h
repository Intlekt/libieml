#pragma once

#include <sstream>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Constants.h"

namespace ieml::AST {

class ComponentDeclaration: public AST, public IDeclaration, public ITranslatable {
public:
    ComponentDeclaration(CharRange char_range, 
                         const TranslationsMap & translations) : 
        AST(char_range), 
        IDeclaration(DeclarationType(Component)), 
        ITranslatable(translations) {};

    std::string to_string() const {
        std::ostringstream os;
        os << "@component " << translations_to_string() << " " << " .";
        return os.str();
    };
};

}
