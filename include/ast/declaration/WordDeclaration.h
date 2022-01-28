#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/Word.h"


namespace ieml::AST {

class WordDeclaration: public virtual AST, public IDeclaration {
public:
    WordDeclaration(std::shared_ptr<CharRange>&& char_range, 
                    std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),          
        IDeclaration(DeclarationType::WORD), 
        word_(std::move(word)) {};


    virtual std::string to_string() const override;

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

    virtual std::string getDeclarationString() const override {return "@word";};

private:
    std::shared_ptr<Word> word_;
};

}