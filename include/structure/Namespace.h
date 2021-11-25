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

    void define(std::shared_ptr<Name> name, std::shared_ptr<Phrase> pharse) {
        for (auto& n: *name) {
            store_.insert({n.second, pharse});
        }
        rev_store_.insert({pharse, name});
    } 

    std::shared_ptr<Phrase> resolve(const LanguageString& ls) {
        // issue when ls not in store_ => call the default constructor of Phrase ...
        return store_[ls];
    };
private:

    template<class T> struct shared_ptr_hash {
        size_t operator()(const std::shared_ptr<T>& e) const {
            return std::hash<T>{}(*e);
        }
    };

    std::unordered_map<LanguageString, std::shared_ptr<Phrase>> store_;
    std::unordered_map<std::shared_ptr<Phrase>, std::shared_ptr<Name>, shared_ptr_hash<Phrase>> rev_store_;
};

}