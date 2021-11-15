#pragma once

#include <map>
#include <string>
#include <vector>

#include "ast/Constants.h"


namespace ieml {
namespace AST {

class ITranslatable {
public:
// is the copy of translations a deep copy ?
    ITranslatable(const TranslationsMap & translations) : 
        translations_(translations) {};


    std::string translations_to_string() const {
        std::ostringstream os;
        bool first = true;
        for (auto const& x : translations_) {
            if (first) {
                first = false;
            } else {
                os << " ";
            }
            
            for (const Identifier& s: x.second) {
                os << languages_names[x.first] << '"' << s.to_string() << '"';
            }
        }
        return os.str();
    };

private:
    const TranslationsMap translations_;

};

}
}