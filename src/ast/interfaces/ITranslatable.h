#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "ast/Constants.h"
#include "ast/LanguageString.h"

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

private:
    std::vector<std::unique_ptr<LanguageString>> translations_;

};

}
}