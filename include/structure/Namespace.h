#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <memory>
#include <stdexcept>

#include "ast/interfaces/AST.h"
#include "structure/Constants.h"
#include "structure/Phrase.h"
#include "structure/LanguageString.h"

namespace ieml::structure {

class Name : public std::unordered_map<LanguageType, LanguageString> {
public:
    Name(std::unordered_set<LanguageString> traductions) : 
        std::unordered_map<LanguageType, LanguageString>(build_traductions(traductions)) {}
    
private:
    static std::unordered_map<LanguageType, LanguageString> build_traductions(std::unordered_set<LanguageString> traductions);
};

class Namespace {
public:

    void define(Name name, Phrase pharse) {
        for (auto& n: name) {
            store_.insert({n.second, pharse});
        }
        rev_store_.insert({pharse, name});
    } 

    const Phrase& resolve(const LanguageString& ls) {
        // issue when ls not in store_ => call the default constructor of Phrase ...
        return store_[ls];
    };
private:
    std::unordered_map<LanguageString, Phrase> store_;
    std::unordered_map<Phrase, Name> rev_store_;
};

}