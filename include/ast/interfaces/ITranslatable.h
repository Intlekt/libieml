#pragma once

#include <map>
#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

#include "ast/Constants.h"
#include "ast/LanguageString.h"
#include "structure/Namespace.h"
#include "structure/LanguageString.h"
#include "structure/Constants.h"

#include "ParserContext.h"

namespace ieml {
namespace AST {


class ITranslatable {
public:
// is the copy of translations a deep copy ?
    ITranslatable(std::vector<std::unique_ptr<LanguageString>> && translations) : 
        translations_(std::move(translations)) {};


    std::string translations_to_string() const {
        std::ostringstream os;
        bool first = true;
        for (auto && x : translations_) {
            if (first) {
                first = false;
            } else {
                os << " ";
            }
            os << x->to_string();            
        }
        return os.str();
    };

    std::shared_ptr<structure::Name> check_translatable(parser::ParserContext& ctx) {
        std::unordered_set<structure::LanguageString> names;
        for (auto&& ls: translations_) {
            const Identifier& language = ls->language();
            
            auto lt = structure::LanguageType::_from_string_nocase_nothrow(language.getName().c_str());
            if (!lt) {
                ctx.getErrorManager().visitorError(
                    language.getCharRange(), 
                    "Invalid language identifier for language string, got: '" + language.getName() + "'."
                );
            } else {
                names.insert(structure::LanguageString(*lt, ls->identifier().getName()));
            }
        } 

        return std::make_shared<structure::Name>(names);
    };   


private:
    std::vector<std::unique_ptr<LanguageString>> translations_;

};

}
}