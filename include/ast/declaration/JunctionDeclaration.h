#pragma once


#include "ast/declaration/ToolWordDeclaration.h"


namespace ieml::AST {

class JunctionDeclaration: public ToolWordDeclaration {
public:
    JunctionDeclaration(std::shared_ptr<CharRange>&& char_range, 
                        std::vector<std::shared_ptr<LanguageString>>&& translations,
                        std::shared_ptr<Word>&& word) : 
        AST(std::move(char_range)),
        ToolWordDeclaration(nullptr, std::move(translations), std::move(word), DeclarationType::JUNCTION) {}
    
    std::string to_string() const {
        return getDeclarationString() + " " + translations_to_string() + " " + word_->to_string() + " .";
    };
    virtual std::string getDeclarationString() const override {return "@junction";};

    void check_declaration(ieml::parser::ParserContextManager& ctx) override {
        auto name = check_translatable(ctx);
        auto word = word_->check_word(ctx);

        if (!name | !word) {
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
    };
};


}