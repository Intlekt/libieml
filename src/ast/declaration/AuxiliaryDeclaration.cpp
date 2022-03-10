#include "ast/declaration/AuxiliaryDeclaration.h"


using namespace ieml::AST;


std::string AuxiliaryDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + accepted_role_type_->to_string() + " " + word_->to_string() + " .";
}

void AuxiliaryDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto name = check_translatable(ctx);
    
    auto role_type = accepted_role_type_->check_role_type(ctx);
    auto word = word_->check_word(ctx);

    if (!name || !word || !role_type) {
        return;
    }
    
    auto auxiliary_word = std::make_shared<structure::AuxiliaryWord>(word->getScript(), *role_type);

    auto& wregister = ctx.getWordRegister();
    if (wregister.word_is_defined(auxiliary_word)) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Cannot redefine word " + word->to_string() + " as an auxiliary, it has already been defined before."
        );
        return;
    }

    ctx.getSourceMapping().register_mapping(auxiliary_word, this);
    wregister.define_auxiliary(name, auxiliary_word);
}
