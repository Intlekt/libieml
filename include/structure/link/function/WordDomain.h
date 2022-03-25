#pragma once


#include <unordered_map>
#include <unordered_set>
#include <list>
#include "structure/script/Script.h"
#include "structure/link/function/WordCondition.h"

#include "ast/macro_utils.h"


namespace ieml::structure {

class WordRegister;

class WordDomain : public std::unordered_map<std::string, Script::Ptr> {
public:
    IEML_DECLARE_PTR_TYPE_STRUCTURE(WordDomain)

    std::unordered_set<std::string> getVariableNameSet() const;

    /**
     * @brief Cartesian product of the variable domains. Each value in the list is 
     * 
     * @return std::list<std::unordered_map<std::string, Script::Ptr>> 
     */
    std::vector<WordCondition::VariableValuation> getValues(WordRegister& wreg) const;
};

}