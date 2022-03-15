#include "ast/declaration/RootParadigmDeclaration.h"


using namespace ieml::AST;


std::string RootParadigmDeclaration::to_string() const {
    return getDeclarationString() + " type:" + root_type_->to_string() + " " + word_->to_string() + " .";
}

void RootParadigmDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    auto word = word_->check_word(ctx);

    auto root_type = structure::WordType::_from_string_nothrow(root_type_->getName().c_str());
    if (!root_type) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Invalid identifier to specify the root paradigm type, expected 'CATEGORY', 'INFLECTION', "
            "'AUXILIARY' or 'JUNCTION'. Got: '" + root_type_->getName() + "'."
        );
        return;
    } 

    if (!word) {
        return;
    }

    const auto script = word->getScript();

    if (script->get_multiplicity() == 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Invalid script for root paradigm definition, not a paradigm "+word->to_string()+"."
        );
        return;
    }

    auto& wregister = ctx.getWordRegister();


    for (auto& ss : script->singular_sequences()) {
        if (wregister.is_declared(ss)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Cannot declare script '" + ss->to_string() + "' as a member of root paradigm '" +script->to_string()+"', it "
                "has already been declared before."
            );
            return;
        }
    }

    for (auto& ss : script->singular_sequences()) {
        wregister.declare_script(ss, *root_type);

        // for categories, also define the word, because there is no special declaration for 
        // category word.
        if (*root_type == +ieml::structure::WordType::CATEGORY)
            wregister.define_word(std::make_shared<structure::CategoryWord>(ss));
    }

    // register the paradigm word into the mapping
    // TODO: change that to be able to use directly the script, because a Word with a 
    // paradigmatic script is not valid.
    ctx.getSourceMapping().register_mapping(word, this);
}
