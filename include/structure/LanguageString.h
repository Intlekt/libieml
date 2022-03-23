#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "structure/Constants.h"
#include "utils.h"


namespace ieml::structure {

class LanguageString {
public:
    LanguageString(LanguageType language, const std::string& value) : 
        language_(language), value_(value) {}

    bool operator==(const LanguageString& rhs) const {
        return language_ == rhs.language_ && value_ == rhs.value_;
    };
    bool operator!=(const LanguageString& rhs) const {
        return language_ != rhs.language_ || value_ != rhs.value_;
    };

    LanguageType language() const {return language_;}
    const std::string& value() const {return value_;}

private:
    const LanguageType language_;
    const std::string value_;
};

class TemplateLanguageString {
public:
    typedef std::shared_ptr<TemplateLanguageString> Ptr;

    // fillers.size() == variables.size() - 1 
    TemplateLanguageString(LanguageType language, 
                           const std::vector<std::string>& fillers,
                           const std::vector<std::string>& variables) :
        language_(language),
        fillers_(fillers),
        variables_(variables) {}
    
    LanguageType language() const {return language_;}
    std::string value(const std::unordered_map<std::string, std::string>& variable_to_value) const {
        std::ostringstream os;
        os << fillers_[0];
        for (size_t i = 0; i < variables_.size(); i++) {
            const auto& v = variable_to_value.find(variables_[i]);
            if (v == variable_to_value.end())
                throw "Missing argument " + variables_[i];
            os << " " << v->second << " " << fillers_[i + 1];
        }
        return os.str();
    }

    std::string to_string() const {
        std::unordered_map<std::string, std::string> mapping;
        for (const auto& v : variables_)
            mapping.insert({v, v});

        return value(mapping);
    }

    bool operator==(const TemplateLanguageString& rhs) const {
        return language_ == rhs.language_ && fillers_ == rhs.fillers_ && variables_ == rhs.variables_;
    };
    bool operator!=(const TemplateLanguageString& rhs) const {
        return language_ != rhs.language_ || fillers_ != rhs.fillers_ || variables_ != rhs.variables_;
    };

    auto getFillers() const {return fillers_;};
    auto getVariables() const {return variables_;};
private:
    const LanguageType language_;
    const std::vector<std::string> fillers_;
    const std::vector<std::string> variables_;

    static std::unordered_map<std::string, size_t> variable_to_pos_from_variables_list(const std::vector<std::string>& variables);
};
}

namespace std {
template<>
struct hash<ieml::structure::LanguageString>
{
    size_t operator()(const ieml::structure::LanguageString& s) const noexcept
    {
        size_t a = hash<ieml::structure::LanguageType>{}(s.language());
        size_t b = hash<string>{}(s.value());
        return a ^ (b << 1);
    }
};

template<>
struct hash<ieml::structure::TemplateLanguageString>
{
    size_t operator()(const ieml::structure::TemplateLanguageString& s) const noexcept
    {
        size_t res = 0;
        hash_combine(res, s.language());
        for (const auto& f: s.getFillers())
            hash_combine(res, f);
        for (const auto& v: s.getVariables())
            hash_combine(res, v);
        return res;
    }
};


}