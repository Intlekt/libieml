#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <stdexcept>

#include "utils.h"
#include "structure/Constants.h"
#include "structure/LanguageString.h"

namespace ieml::structure {

class Name : public std::map<LanguageType, LanguageString> {
public:
    Name(std::unordered_set<LanguageString> traductions) : 
         std::map<LanguageType, LanguageString>(build_traductions(traductions)) {}
    
    bool operator==(const Name& rhs) const {
        return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
    }
private:
    static std::map<LanguageType, LanguageString> build_traductions(std::unordered_set<LanguageString> traductions);
};

template<typename V>
class Namespace {
public:

    void define(std::shared_ptr<Name> name, std::shared_ptr<V> value) {
        for (auto& n: *name) {
            store_.insert({n.second, value});
        }
        rev_store_.insert({value, name});
    } 

    std::shared_ptr<V> resolve(const LanguageString& ls) const {
        auto res = store_.find(ls);
        if (res != store_.end()) {
            return res->second;
        }
        return nullptr;
    };
    
private:
    std::unordered_map<LanguageString, std::shared_ptr<V>> store_;
    std::unordered_map<std::shared_ptr<V>, std::shared_ptr<Name>> rev_store_;
};

}