#pragma once

#include <string>

#include "structure/Constants.h"


namespace ieml::structure {

class LanguageString {
public:
    LanguageString(LanguageType language, const std::string& value) : 
        language_(language), value_(value) {}

    bool operator==(const LanguageString& rhs) const {
        return language_ == rhs.language_ && value_ == rhs.value_;
    };

    LanguageType language() const {return language_;}
    const std::string& value() const {return value_;}

private:
    const LanguageType language_;
    const std::string value_;
};
}

namespace std {
template<>
struct hash<ieml::structure::LanguageString>
{
    size_t operator()(const ieml::structure::LanguageString& s) const noexcept
    {
        size_t a = std::hash<ieml::structure::LanguageType>{}(s.language());
        size_t b = std::hash<std::string>{}(s.value());
        return a ^ (b << 1);
    }
};


}