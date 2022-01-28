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
    if (ctx.getCategoryRegister().category_is_defined(phrase)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot redefine phrase " + phrase->to_string() + ", it has already been defined before."
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

    ctx.getSourceMapping().register_mapping(phrase, this);
    define_category(ctx, name, phrase);
}