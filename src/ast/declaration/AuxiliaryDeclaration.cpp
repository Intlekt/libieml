#include "ast/declaration/AuxiliaryDeclaration.h"


using namespace ieml::AST;


std::string AuxiliaryDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + accepted_role_type_->to_string() + " " + word_->to_string() + " .";
}

void AuxiliaryDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto name = check_translatable(ctx);
    auto role_type = accepted_role_type_->check_role_type(ctx);
    auto script = word_->check_is_not_defined(ctx, structure::WordType::AUXILIARY);

    if (!name || !script || !role_type) {
        return;
    }
    
    auto auxiliary_word = std::make_shared<structure::AuxiliaryWord>(script, *role_type);
    ctx.getSourceMapping().register_mapping(auxiliary_word, this);
    ctx.getWordRegister().define_auxiliary(name, auxiliary_word);
}
