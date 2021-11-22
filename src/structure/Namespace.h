#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <set>

namespace ieml::structure {

namespace constants {
    enum LanguageType {FR = 0, EN};

    const std::unordered_map<std::string, LanguageType> languageStr_to_LanguageType = {{"fr", LanguageType(FR)}, {"en", LanguageType(EN)}};
    const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};
}

class Name {
public:
    Name(std::set<LanguageString> traductions) : traductions_(build_traductions(traductions)) {}


private:
    std::unordered_map<constants::LanguageType, LanguageString> traductions_;

    static std::unordered_map<constants::LanguageType, LanguageString> build_traductions(std::vector<LanguageString> traductions) {
        std::unordered_map<constants::LanguageType, LanguageString> traductions_;
        for (const auto& item: traductions) {
            traductions_.insert({item.language(), item});
        }

    }
};

class LanguageString {
public:
    LanguageString(constants::LanguageType language, const std::string& value) : 
        language_(language), value_(value) {}

    constants::LanguageType language() const {return language_;}

private:
    const constants::LanguageType language_;
    const std::string value_;
};

template <class T>
class Namespace {
public:

    T resolve(LanguageType language, const std::string& value) const {
        return store_[language][value];
    };
private:
    std::unordered_map<Name, T> store_;
    std::unordered_map<T, Name> rev_store_;

};

}