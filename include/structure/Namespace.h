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

    void define(std::shared_ptr<Name> name, std::shared_ptr<Phrase> phrase, bool is_node) {
        for (auto& n: *name) {
            store_.insert({n.second, phrase});
        }
        rev_store_.insert({phrase, name});
        is_node_.insert({phrase, is_node});
    } 

    std::shared_ptr<Phrase> resolve(const LanguageString& ls) {
        auto res = store_.find(ls);
        if (res != store_.end()) {
            return res->second;
        }
        return nullptr;
    };
    
private:

    template<class T> struct shared_ptr_hash {
        size_t operator()(const std::shared_ptr<T>& e) const {
            return std::hash<T>{}(*e);
        }
    };

    std::unordered_map<LanguageString, std::shared_ptr<Phrase>> store_;
    std::unordered_map<std::shared_ptr<Phrase>, std::shared_ptr<Name>, shared_ptr_hash<Phrase>> rev_store_;
    std::unordered_map<std::shared_ptr<Phrase>, bool, shared_ptr_hash<Phrase>> is_node_;
};

}