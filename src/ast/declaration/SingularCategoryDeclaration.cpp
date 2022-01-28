#include "ast/declaration/SingularCategoryDeclaration.h"


using namespace ieml::AST;


void SingularCategoryDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                std::shared_ptr<structure::Name>& name, 
                                std::shared_ptr<structure::PathTree>& phrase) const {
    ctx.getCategoryRegister().define_category(name, phrase, 
        (getDeclarationType() == +DeclarationType::NODE ? 
            structure::DefinitionType::NODE : 
            structure::DefinitionType::COMPONENT)
    );
}

std::shared_ptr<ieml::structure::PathTree> SingularCategoryDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
    auto phrase_set = getPhrase()->check_phrase(ctx);

    if (phrase_set.size() != 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Paradigms can only be defined with a @paranode declaration."
        );
        return nullptr;
    }

    return *phrase_set.begin();
}
