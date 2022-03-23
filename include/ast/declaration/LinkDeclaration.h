#pragma once


#include "ast/declaration/CategoryDeclaration.h"
#include "ast/Word.h"
#include "ast/ArgumentList.h"
#include "ast/TemplateLanguageString.h"

#include <vector>

namespace ieml::AST {
class LinkDeclaration: public virtual AST, public CategoryDeclaration {
public:
    LinkDeclaration(CharRange::Ptr&& char_range,
                    ArgumentList::Ptr&& arguments,
                    std::vector<std::shared_ptr<LanguageString>>&& translations,
                    std::vector<TemplateLanguageString::Ptr>&& template_translations,
                    IInflectionList::Ptr&& phrase_word_inflection_list,
                    std::shared_ptr<Phrase>&& phrase) :
        AST(std::move(char_range)),
        CategoryDeclaration(nullptr, std::move(translations), std::move(phrase), DeclarationType::LINK),
        arguments_(std::move(arguments)),
        template_translations_(std::move(template_translations)),
        phrase_word_inflection_list_(std::move(phrase_word_inflection_list)) {}

    virtual std::string getDeclarationString() const override {return "@link";};

protected:
    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 const PartialPathTree& phrase) const override;

    virtual PartialPathTree::Optional _check_phrase(ieml::parser::ParserContextManager& ctx) const override;

private:
    const ArgumentList::Ptr arguments_;
    const std::vector<TemplateLanguageString::Ptr> template_translations_;
    const IInflectionList::Ptr phrase_word_inflection_list_;
};
}