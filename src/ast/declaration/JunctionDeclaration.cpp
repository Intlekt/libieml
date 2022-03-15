#include "ast/declaration/JunctionDeclaration.h"


using namespace ieml::AST;


std::string JunctionDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + word_->to_string() + " .";
}

void JunctionDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto name = check_translatable(ctx);
    auto script = word_->check_is_not_defined(ctx, structure::WordType::JUNCTION);
    if (!name || !script) {
        return;
    }
    auto junction_word = std::make_shared<structure::JunctionWord>(script);

    ctx.getSourceMapping().register_mapping(junction_word, this);
    ctx.getWordRegister().define_junction(name, junction_word);
}