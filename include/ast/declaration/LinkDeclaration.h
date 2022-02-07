#pragma once


#include "ast/declaration/CategoryDeclaration.h"
#include "ast/Word.h"


namespace ieml::AST {
class LinkDeclaration: public virtual AST, public CategoryDeclaration {
public:
    LinkDeclaration(std::shared_ptr<CharRange>&& char_range,
                    std::vector<std::shared_ptr<LanguageString>>&& translations,
                    std::shared_ptr<Phrase>&& phrase) :
        AST(std::move(char_range)),
        CategoryDeclaration(nullptr, std::move(translations), std::move(phrase), DeclarationType::LINK) {}

    virtual std::string getDeclarationString() const override {return "@link";};

protected:
    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 const PartialPathTree& phrase) const override;

    virtual PartialPathTree::Optional _check_phrase(ieml::parser::ParserContextManager& ctx) const override;

private:
    const std::shared_ptr<Phrase> phrase_;
};
}