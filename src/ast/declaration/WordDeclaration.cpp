#include "ast/declaration/WordDeclaration.h"


using namespace ieml::AST;


std::string WordDeclaration::to_string() const {
    return getDeclarationString() + " " + word_->to_string() + " .";
}

void WordDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto word = word_->check_word(ctx);

    if (!word) {
        return;
    }

    auto& wregister = ctx.getWordRegister();

    if (wregister.word_is_defined(word)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot redefine word " + word->to_string() + " as a category word, it has already been defined before."
        );
        return;
    }

    ctx.getSourceMapping().register_mapping(word, this);
    wregister.define_word(word);
}
