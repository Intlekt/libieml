#include "ast/declaration/WordDeclaration.h"


using namespace ieml::AST;


std::string WordDeclaration::to_string() const {
    return getDeclarationString() + " " + word_->to_string() + " .";
}

void WordDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto script = word_->check_script(ctx);

    if (!script) {
        return;
    }

    auto& wregister = ctx.getWordRegister();

    if (wregister.script_is_declared(script)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Already declared script \"" + script->to_string() + "\". Cannot redeclare it."
        );
        return;
    }

    if (wregister.word_is_defined(script)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot redefine word \"" + script->to_string() + "\" as a category word, it has already been defined before."
        );
        return;
    }

    auto word = std::make_shared<structure::CategoryWord>(script);
    ctx.getSourceMapping().register_mapping(word, this);
    wregister.declare_script(script, structure::WordType::CATEGORY);
    wregister.define_word(word);
}
