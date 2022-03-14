#include "ast/declaration/JunctionDeclaration.h"


using namespace ieml::AST;


std::string JunctionDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + word_->to_string() + " .";
}

void JunctionDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto name = check_translatable(ctx);
    auto word = word_->check_word(ctx);

    if (!name || !word) {
        return;
    }
    
    auto junction_word = std::make_shared<structure::JunctionWord>(word->getScript());

    auto& wregister = ctx.getWordRegister();
    if (wregister.word_is_defined(junction_word)) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Cannot redefine word " + word->to_string() + " as a junction, it has already been defined before."
        );
        return;
    }

    ctx.getSourceMapping().register_mapping(junction_word, this);
    wregister.define_junction(name, junction_word);
}