#pragma once

#include "ast/declaration/CategoryDeclaration.h"
#include "ast/DimensionDefinition.h"


namespace ieml::AST {
class ParanodeDeclaration: public CategoryDeclaration {
public:
    ParanodeDeclaration(std::shared_ptr<CharRange>&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::vector<std::shared_ptr<DimensionDefinition>>&& dimensions,
                        std::shared_ptr<Phrase>&& phrase) : 
        AST(std::move(char_range)), 
        CategoryDeclaration(
            nullptr,
            std::move(translations),
            std::move(phrase),
            DeclarationType::PARANODE),
        dimension_definitions_(std::move(dimensions)) {};

    virtual std::string getDeclarationString() const override {return "@paranode";};

    virtual void define_category(ieml::parser::ParserContextManager& ctx, 
                                 std::shared_ptr<structure::Name>& name, 
                                 std::shared_ptr<structure::PathTree>& phrase) const override {
        
        auto invariant = ieml::structure::PathTree::paradigm_invariant(ctx.getPathTreeRegister(), phrase);
        if (invariant == nullptr) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot define a paradigm with an empty invariant."
            );
            return;
        }

        if (!ctx.getCategoryRegister().category_is_defined(invariant)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot define a paradigm when its node invariant has not been defined."
            );
            return;
        }

        if (ctx.getCategoryRegister().getDefinitionType(invariant) != +ieml::structure::DefinitionType::NODE) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "The invariant of this paradigm has not been defined as a node."
            );
            return;
        }

        if (ctx.getParadigmRegister().resolve_paradigm(invariant)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "A paradigm with the same node invariant has already been defined."
            );
            return;
        }

        ctx.getCategoryRegister().define_category(name, phrase, structure::DefinitionType::PARADIGM);
        ctx.getParadigmRegister().define_paradigm(ctx.getPathTreeRegister(), phrase);
    }

protected:
    virtual std::shared_ptr<structure::PathTree> _check_phrase(ieml::parser::ParserContextManager& ctx) const override {
        auto phrase_list = getPhrase()->check_phrase(ctx);

        if (phrase_list.size() == 1) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "A @paranode declaration must define a paradigm category."
            );
            return nullptr;
        }

        structure::PathTree::Set children;

        for (size_t i = 0; i < phrase_list.size(); ++i) {
            children.insert({
                ctx.getPathTreeRegister().get_or_create(
                    std::make_shared<structure::ParadigmIndexPathNode>(i), 
                    {phrase_list[i]}
                )});
        }

        return ctx.getPathTreeRegister().get_or_create(
            std::make_shared<structure::ParadigmPathNode>(), 
            children);
    };

    std::vector<structure::PathTree::Set> _check_dimension_definitions(__attribute__((unused)) ieml::parser::ParserContextManager& ctx) const {
        return {};
    }

private:
    const std::vector<std::shared_ptr<DimensionDefinition>> dimension_definitions_;

};


}