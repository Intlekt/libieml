#pragma once


#include "ast/declaration/ToolWordDeclaration.h"


namespace ieml::AST {

class AuxiliaryDeclaration: public ToolWordDeclaration {
public:
    AuxiliaryDeclaration(std::shared_ptr<CharRange>&& char_range, 
                         std::vector<std::shared_ptr<LanguageString>>&& translations,
                         int accepted_role_type,
                         std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::AUXILIARY),
        accepted_role_type_(accepted_role_type) {}
    
    std::string to_string() const {
        return getDeclarationString() + " " + translations_to_string() + " " + std::to_string(accepted_role_type_) + " " + word_->to_string() + " .";
    };

    virtual std::string getDeclarationString() const override {return "@auxiliary";};

    void check_declaration(ieml::parser::ParserContextManager& ctx) override {
        auto name = check_translatable(ctx);

        auto role_type = structure::RoleType::_from_integral_nothrow(accepted_role_type_);
        if (!role_type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role number for auxiliary declaration, got '" + std::to_string(accepted_role_type_) + "'"
            );
            return;
        }

        auto word = word_->check_word(ctx);

        if (!name | !word | !role_type) {
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
    };

private:
    const int accepted_role_type_;
};




}