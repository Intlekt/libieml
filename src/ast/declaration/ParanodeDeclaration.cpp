#include "ast/declaration/ParanodeDeclaration.h"


using namespace ieml::AST;

void ParanodeDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                          std::shared_ptr<structure::Name>& name, 
                                          std::shared_ptr<structure::PathTree>& phrase) const {
    
    auto& reg = ctx.getPathTreeRegister();
    auto invariant = reg.buildFromPaths(reg.invariant_paths(phrase));
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

    const auto& dimensions = _check_dimension_definitions(ctx, phrase);
    ctx.getParadigmRegister().define_paradigm(ctx.getPathTreeRegister(), phrase, dimensions);
}

std::shared_ptr<ieml::structure::PathTree> ParanodeDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
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
}

std::vector<ieml::structure::PathTree::Set> ParanodeDeclaration::_check_dimension_definitions(ieml::parser::ParserContextManager& ctx, 
                                                                                              const std::shared_ptr<structure::PathTree>& phrase) const {
    std::vector<ieml::structure::PathTree::Set> dimensions;
    for (auto dim: dimension_definitions_)
        dimensions.push_back(dim->check_dimension_definitions(ctx, phrase));

    return dimensions;
}
