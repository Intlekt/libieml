#include "ast/declaration/ParanodeDeclaration.h"


using namespace ieml::AST;

void ParanodeDeclaration::define_category(ieml::parser::ParserContextManager& ctx, 
                                          std::shared_ptr<structure::Name>& name, 
                                          const PartialPathTree& phrase) const {
    
    auto phrase_pt = *phrase.getPathTrees().begin();

    auto& reg = ctx.getPathTreeRegister();
    auto invariant = reg.buildFromPaths(reg.invariant_paths(phrase_pt));
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

    ctx.getCategoryRegister().define_category(name, phrase_pt, structure::DefinitionType::PARADIGM);    

    const auto& dimensions = _check_dimension_definitions(ctx, phrase_pt);
    if (!dimensions)
        return;
    
    ctx.getParadigmRegister().define_paradigm(ctx.getPathTreeRegister(), phrase_pt, *dimensions);
}

PartialPathTree::Optional ParanodeDeclaration::_check_phrase(ieml::parser::ParserContextManager& ctx) const {
    auto phrase_list = getPhrase()->check_phrase(ctx);

    if (!phrase_list)
        return {};

    if (phrase_list->getPathTrees().size() == 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "A @paranode declaration must define a paradigm category."
        );
        return {};
    }

    structure::PathTree::Set children;

    for (size_t i = 0; i < phrase_list->getPathTrees().size(); ++i) {
        children.insert({
            ctx.getPathTreeRegister().get_or_create(
                std::make_shared<structure::ParadigmIndexPathNode>(i), 
                {phrase_list->getPathTrees()[i]}
            )});
    }

    if (phrase_list->getReferences().size() != 0) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Paradigms cannot have references."
        );
        return {};
    }

    return PartialPathTree({ctx.getPathTreeRegister().get_or_create(
                                std::make_shared<structure::ParadigmPathNode>(), 
                                children)}, {});
}

std::optional<std::vector<ieml::structure::PathTree::Set>> ParanodeDeclaration::_check_dimension_definitions(ieml::parser::ParserContextManager& ctx, 
                                                                                                             const std::shared_ptr<structure::PathTree>& phrase) const {
    std::vector<ieml::structure::PathTree::Set> dimensions;
    std::set<size_t> seen; 

    bool valid = true;

    for (auto dim: dimension_definitions_) {
        const auto dim_checked = dim->check_dimension_definitions(ctx, phrase);
        if (dim_checked.size() == 0) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "No path are defined for the " + std::to_string(dim->getDimensionIndex()) + "d dimension."
            );

            valid = false;
            continue;
        }

        size_t total_variant_size = 0;
        const auto& invariant_path = ctx.getPathTreeRegister().invariant_paths(phrase);

        for (auto& ss : ctx.getPathTreeRegister().expand_path(phrase, dim_checked)) {
            total_variant_size += std::count_if(
                ss.begin(), 
                ss.end(), 
                [&invariant_path](const auto& path){return invariant_path.count(path) == 0;}
            );
        }


        if (total_variant_size < 2) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "The " + std::to_string(dim->getDimensionIndex()) + "d dimension does not reference one or more opposition group in the paradigm."
            );

            valid = false;
            continue;
        }

        dimensions.push_back(dim_checked);

        const auto it = seen.find(dim->getDimensionIndex());
        if (it != seen.end()) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "The dimension definition " + std::to_string(dim->getDimensionIndex()) + "d is already defined."
            );
            valid = false;
            continue;
        }
        seen.insert(dim->getDimensionIndex());
    }

    if (seen.size() > 2) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Too many dimension definitions, cannot defines more than three dimensions."
        );
        valid = false;
    } 

    for (size_t i = 0; i < seen.size(); ++i) {
        const auto it = seen.find(i + 1);
        if (it == seen.end()) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Invalid dimension definitions, missing the " + std::to_string(i + 1) + "d dimension."
            );
            valid = false;
        }
    }

    if (!valid)
        return {};

    return dimensions;
}
