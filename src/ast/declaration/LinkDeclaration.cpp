#include "ast/declaration/LinkDeclaration.h"


using namespace ieml::AST;


void LinkDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                      std::shared_ptr<ieml::structure::Name>& name, 
                                      const PartialPathTree& phrase) const {
    // should defines the phrase and its schema
    auto phrase_pt = *phrase.getPathTrees().begin();

    ctx.getCategoryRegister().define_link(name, phrase_pt);
}

PartialPathTree::Optional LinkDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
    auto phrase_set = getPhrase()->check_phrase(ctx);
    if (!phrase_set)
        return {};

    if (phrase_set->getPathTrees().size() != 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Links cannot be a paradigm."
        );
        return {};
    }

    if (phrase_set->getReferences().size() == 0) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot defines a link without references."
        );
        return {};
    }

    return phrase_set;
}