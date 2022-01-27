#pragma once


#include "ast/declaration/SingularCategoryDeclaration.h"


namespace ieml::AST {

class ComponentDeclaration: public SingularCategoryDeclaration {
public:
    ComponentDeclaration(std::shared_ptr<CharRange>&& char_range, 
                         std::vector<std::shared_ptr<LanguageString>>&& translations,
                         std::shared_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        SingularCategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            DeclarationType::COMPONENT) {};
    
    virtual std::string getDeclarationString() const override {return "@component";};

};

}