#include "ast/declaration/LanguageDeclaration.h"


using namespace ieml::AST;


std::string LanguageDeclaration::to_string() const {
    return getDeclarationString() + " " + language_type_->to_string() + " .";
}

void LanguageDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto language = ieml::structure::LanguageType::_from_string_nocase_nothrow(language_type_->getName().c_str());

    if (!language) {
        ctx.getErrorManager().visitorError(
            language_type_->getCharRange(), 
            "Invalid language identifier for language declaration, got: '" + language_type_->getName() + "'."
        );
        return;
    }
    auto is_set = ctx.setLanguage(*language);
    if (!is_set) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Cannot redefine language of the file. It has already been defined before."
        );
        return;
    }

    return;
}