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

#include "ast/macro_utils.h"

namespace ieml::structure {

class Name : public std::unordered_multimap<LanguageType, LanguageString> {
public:
    IEML_DECLARE_PTR_TYPE_STRUCTURE(Name)

    typedef std::unordered_multiset<LanguageString> Traductions;

    Name(Traductions traductions) : 
         std::unordered_multimap<LanguageType, LanguageString>(build_traductions(traductions)) {}
    
    bool operator==(const Name& rhs) const {
        return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
    }
private:
    static std::unordered_multimap<LanguageType, LanguageString> build_traductions(std::unordered_multiset<LanguageString> traductions);
};

class TemplateName : public std::unordered_multimap<LanguageType, TemplateLanguageString> {
public:

    typedef std::unordered_multiset<TemplateLanguageString> TemplateTraductions;

    TemplateName(TemplateTraductions traductions) : 
         std::unordered_multimap<LanguageType, TemplateLanguageString>(build_traductions(traductions)) {}
    
    bool operator==(const TemplateName& rhs) const {
        return size() == rhs.size() && std::equal(begin(), end(), rhs.begin());
    }

    Name getNameForValuation(std::unordered_map<std::string, Name::Ptr>) const;
private:
    static std::unordered_multimap<LanguageType, TemplateLanguageString> build_traductions(std::unordered_multiset<TemplateLanguageString> traductions);
};

template<typename V>
class Namespace : public std::unordered_map<std::shared_ptr<V>, std::shared_ptr<Name>> {
public:

    void define(const std::shared_ptr<Name>& name, const std::shared_ptr<V>& value) {
        for (const auto& n: *name) {
            store_.insert({n.second, value});
        }
        this->insert(std::pair<std::shared_ptr<V>, std::shared_ptr<Name>>{value, name});
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
};

}