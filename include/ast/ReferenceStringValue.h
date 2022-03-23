#pragma once

#include <memory>
#include <regex>


#include "ast/interfaces/AST.h"
#include "ast/interfaces/IReferenceValue.h"


namespace ieml::AST {
class ReferenceStringValue: virtual public AST, public IReferenceValue {
public:
    ReferenceStringValue(CharRange::Ptr&& char_range,
                         const std::string& value) :
        AST(std::move(char_range)),
        value_(unescape_value(value)) {}

    std::string to_string() const override {
        return "'" + escaped_value() + "'";
    }

    std::string escaped_value() const {
        std::string s_ = value_;
        s_ = std::regex_replace(s_, std::regex(R"(\\)"), R"(\\\\)");
        s_ = std::regex_replace(s_, std::regex(R"(')"), R"(\\')");
        return s_;
    }

    std::string unescape_value(const std::string& s) const {
        std::string s_ = s.substr(1, s.size() - 2);
        s_ = std::regex_replace(s_, std::regex("\\'"), "'");
        s_ = std::regex_replace(s_, std::regex("\\\\"), "\\");
        return s_;
    }


private:
    const std::string value_;

};
}