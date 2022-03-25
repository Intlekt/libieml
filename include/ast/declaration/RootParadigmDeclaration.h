#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/Word.h"
#include "ast/Identifier.h"


namespace ieml::AST {

class RootParadigmDeclaration: public virtual AST, public IDeclaration {
public:
    RootParadigmDeclaration(CharRange::Ptr&& char_range, 
                            std::shared_ptr<Identifier>&& root_type,
                            std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),          
        IDeclaration(DeclarationType::WORD), 
        root_type_(std::move(root_type)),
        word_(std::move(word)) {};


    virtual std::string to_string() const override;

    virtual void check_declaration(ieml::parser::ParserContextManager& ctx) const override;

    virtual std::string getDeclarationString() const override {return "@rootparadigm";};
    
private:
    std::shared_ptr<Identifier> root_type_;
    std::shared_ptr<Word> word_;
};

}