#include "ast/declaration/SingularCategoryDeclaration.h"


using namespace ieml::AST;


void SingularCategoryDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                                 std::shared_ptr<structure::Name>& name, 
                                                 const PartialPathTree& phrase) const {
    auto phrase_pt = *phrase.getPathTrees().begin();

    ctx.getCategoryRegister().define_category(name, phrase_pt, 
        (getDeclarationType() == +DeclarationType::NODE ? 
            structure::DefinitionType::NODE : 
            structure::DefinitionType::COMPONENT)
    );
}

PartialPathTree::Optional SingularCategoryDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
    auto phrase_set = getPhrase()->check_phrase(ctx);
    if (!phrase_set)
        return {};

    if (phrase_set->getPathTrees().size() != 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Paradigms can only be defined with a @paranode declaration."
        );
        return {};
    }

    return phrase_set;
}
