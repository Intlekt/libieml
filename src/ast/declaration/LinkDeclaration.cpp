#include "ast/declaration/LinkDeclaration.h"
#include "structure/Namespace.h"

#include <unordered_set>

using namespace ieml::AST;


void LinkDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                      std::shared_ptr<ieml::structure::Name>& name, 
                                      const PartialPathTree& phrase) const {
    auto phrase_pt = *phrase.getPathTrees().begin();
    ctx.getCategoryRegister().define_link(name, phrase_pt);
}

PartialPathTree::Optional LinkDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
    const auto phrase_set = getPhrase()->check_phrase(ctx);
    if (!phrase_set)
        return {};

    const auto arguments = arguments_->check_arguments(ctx);
    if (!arguments)
        return {};

    if (template_translations_.size() == 0) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Links must have at least one template translation."
        );
        return {};
    }

    std::unordered_multiset<structure::TemplateLanguageString> tmpl_language_strings;
    for (const auto& t_trans: template_translations_) {
        const auto t = t_trans->check_template_language_string(ctx, *arguments);
        if (!t) 
            return {};
        tmpl_language_strings.insert(*t);
    }
    const structure::TemplateName template_name(tmpl_language_strings);

    ieml::structure::PathNode::Ptr inflection;
    if (phrase_word_inflection_list_) {
        const auto phrase_word_inflection_list = phrase_word_inflection_list_->check_inflection_list(ctx, structure::RoleType::ROOT);
        if (!phrase_word_inflection_list[0]) 
            return {};
        inflection = phrase_word_inflection_list[0];
    }

    // forbid paradigms
    if (phrase_set->getPathTrees().size() != 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Links cannot be a paradigm."
        );
        return {};
    }

    // check arguments existance and completeness of phrase_set

    if (arguments->getVariableNameSet() != phrase_set->getVariableNames()) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Invalid coverage of variable names in the link phrase. Missing variables or extra variables."
        );
        return {};
    }
    
    // register link
    ctx.getLinkRegister().create_link(
        *phrase_set->getPathTrees().begin(),
        *arguments,
        template_name,
        inflection
    );

    // register schema
    const auto schema_args = phrase_set->getReferenceSchemaArguments();
    if (!schema_args) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Incoherent reference schema in link phrase."
        );
        return {};
    }

    ctx.getReferenceSchemaRegister().register_schema(*schema_args);

    return phrase_set;
}