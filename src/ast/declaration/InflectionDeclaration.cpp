#include "ast/declaration/InflectionDeclaration.h"


using namespace ieml::AST;


std::string InflectionDeclaration::to_string() const {
    return getDeclarationString() + " " + translations_to_string() + " " + type_->to_string() + " " + word_->to_string() + " .";
}

void InflectionDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto name = check_translatable(ctx);

    // test if type_ is either VERB or NOUN
    auto inflection_type = structure::InflectionType::_from_string_nothrow(type_->getName().c_str());
    if (!inflection_type) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Invalid identifier to specify the inflection type, expected 'NOUN' or 'VERB', got: '" + type_->getName() + "'."
        );
        return;
    } 

    auto script = word_->check_is_not_defined(ctx, structure::WordType::INFLECTION);
    if (!name || !script || !inflection_type) {
        return;
    }
    
    auto inflection_word = std::make_shared<structure::InflectionWord>(script, *inflection_type);

    ctx.getSourceMapping().register_mapping(inflection_word, this);
    ctx.getWordRegister().define_inflection(name, inflection_word);
}