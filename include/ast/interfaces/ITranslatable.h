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
        std::unordered_multiset<structure::LanguageString> names;

        bool valid = true;
        for (auto&& ls: translations_) {
            auto lt = ls->check_language_string(ctx);
            if (lt)
                names.insert(*lt);
            else
                valid = false;
        } 
        if (!valid)
            return nullptr;

        return std::make_shared<structure::Name>(names);
    };   


private:
    std::vector<std::unique_ptr<LanguageString>> translations_;

};

}
}