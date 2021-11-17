#pragma once

#include <memory>
#include <string>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"


namespace ieml::AST {

class LanguageString: virtual public AST {
public:
    LanguageString(std::unique_ptr<CharRange>&& char_range, 
                   LanguageType language_type, 
                   std::unique_ptr<Identifier>&& identifier) : 
        AST(std::move(char_range)), 
        language_type_(language_type), 
        identifier_(std::move(identifier)) {}

    std::string to_string() const override {
        return languages_names[language_type_] + "\"" + identifier_->to_string() + "\"";
    }

private:
    LanguageType language_type_;
    std::unique_ptr<Identifier> identifier_;
};

}