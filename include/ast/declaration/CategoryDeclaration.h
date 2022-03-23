#pragma once


#include "ast/interfaces/IDeclaration.h"
#include "ast/interfaces/ITranslatable.h"
#include "ast/Phrase.h"


namespace ieml::AST {

class CategoryDeclaration: public virtual AST, public IDeclaration, public ITranslatable {
public:
    CategoryDeclaration(CharRange::Ptr&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::shared_ptr<Phrase>&& phrase,
                        DeclarationType declaration_type) : 
        AST(std::move(char_range)), 
        IDeclaration(declaration_type),
        ITranslatable(std::move(translations)),
        phrase_(std::move(phrase)) {};

    virtual std::string to_string() const override;

    virtual void check_declaration(ieml::parser::ParserContextManager&) const override;

    std::shared_ptr<Phrase> getPhrase() const {return phrase_;};

protected:

    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 const PartialPathTree& phrase) const = 0;

    virtual PartialPathTree::Optional _check_phrase(ieml::parser::ParserContextManager& ctx) const = 0;

private:
    const std::shared_ptr<Phrase> phrase_;
};

}