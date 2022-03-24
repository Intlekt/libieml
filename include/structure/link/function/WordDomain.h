#pragma once


#include <unordered_map>
#include "structure/script/Script.h"

#include "ast/macro_utils.h"


namespace ieml::structure {

class WordDomain : public std::unordered_map<std::string, Script::Ptr> {
public:
    IEML_DECLARE_PTR_TYPE_STRUCTURE(WordDomain)

    std::unordered_set<std::string> getVariableNameSet() const {
        std::unordered_set<std::string> res;
        for (const auto& it : *this) {
            res.insert(it.first);
        }
        return res;
    }
};

}