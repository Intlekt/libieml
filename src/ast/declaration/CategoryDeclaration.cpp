#include "ast/declaration/CategoryDeclaration.h"


using namespace ieml::AST;


std::string CategoryDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + phrase_->to_string() + " .";
}

void CategoryDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto phrase = _check_phrase(ctx);
    auto name = check_translatable(ctx);

    if (!phrase || !name) {
        return;
    }
    bool valid = true;

    if (phrase->getPathTrees().size() != 1)
        throw std::invalid_argument("Only single path trees permitted at this points");
    
    auto phrase_path_tree = *phrase->getPathTrees().begin();
    if (ctx.getCategoryRegister().category_is_defined(phrase_path_tree)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot redefine phrase " + phrase_path_tree->to_string() + ", it has already been defined before."
        );
        valid = false;
    }
    
    for (const auto& ls: *name) {
        if (ctx.getCategoryRegister().resolve_category(ls.second)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot redefine language string '" + std::string(ls.second.language()._to_string()) +
                ":" + ls.second.value() + "' with another category, it has already been defined before."
            );

            valid = false;
        }
    }

    if (!valid)
        return;

    ctx.getSourceMapping().register_mapping(phrase_path_tree, this);
    define_category(ctx, name, *phrase);
}