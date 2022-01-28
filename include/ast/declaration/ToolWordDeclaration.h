#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Word.h"


namespace ieml::AST {

class ToolWordDeclaration: public virtual AST, public IDeclaration, public ITranslatable {
public:
    ToolWordDeclaration(std::shared_ptr<CharRange>&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::shared_ptr<Word>&& word,
                        DeclarationType declaration_type) : 
        AST(std::move(char_range)),
        IDeclaration(declaration_type),
        ITranslatable(std::move(translations)),
        word_(std::move(word)) {};

protected:
    std::shared_ptr<Word> word_;
};

}