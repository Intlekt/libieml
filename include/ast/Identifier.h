#pragma once

#include <memory>
#include <string>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"
#include "ast/interfaces/IReferenceValue.h"
#include "structure/LanguageString.h"


namespace ieml::AST {
class Identifier : virtual public AST, public ICategory, public IReferenceValue {
public:
    Identifier(std::shared_ptr<CharRange>&& char_range, 
               std::string name) : 
        AST(std::move(char_range)),
        ICategory(),
        IReferenceValue(), 
        name_(name) {};
    
    std::string to_string() const override {
        return name_;
    }; 

    const std::string& getName() const {
        return name_;
    }

    virtual structure::PathTree::Vector check_category(parser::ParserContextManager& ctx) const {        
        std::shared_ptr<structure::PathTree> phrase = ctx.getCategoryRegister().resolve_category(structure::LanguageString(ctx.getLanguage(), name_));

        if (phrase == nullptr) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Undefined category identifier '" + name_ + "'."
            );

            return {nullptr};
        }

        return structure::PathTree::singular_sequences(phrase);
    };

private:
    const std::string name_;

};

}