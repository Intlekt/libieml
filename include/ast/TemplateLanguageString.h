#pragma once


#include "ast/interfaces/AST.h"
#include "ast/macro_utils.h"

namespace ieml::AST {
class TemplateLanguageString : public virtual AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(TemplateLanguageString)

    enum class SType {STRING = 0, VARIABLE = 1};

    TemplateLanguageString(CharRange::Ptr&& char_range, 
                           Identifier::Ptr&& language_type, 
                           std::vector<std::pair<SType, std::string>>&& content) : 
        AST(std::move(char_range)), 
        language_type_(std::move(language_type)), 
        content_(std::move(content)) {}

    std::string to_string() const override {
        std::ostringstream os;
        bool first = true;
        for (const auto& v: content_) {
            if (first) first = false;
            else       os << " ";
            os << v.second;
        }

        return "template-" + language_type_->getName() + ": " + os.str();
    }

    const Identifier& language() const {return *language_type_;};

    std::optional<structure::TemplateLanguageString> check_template_language_string(parser::ParserContextManager& ctx, 
                                                                          const structure::LinkArgumentDefinition& args) const {
        const auto lname = language_type_->getName();

        //  9 == "template-".size()
        const auto ltag = lname.substr(9, std::string::npos); // npos indicates until the end

        auto language = structure::LanguageType::_from_string_nocase_nothrow(ltag.c_str());
        if (!language) {
            ctx.getErrorManager().visitorError(
                language_type_->getCharRange(),
                "Invalid language identifier for template language string, got: '" + language_type_->getName() + "'."
            );
            return {};
        }

        std::vector<std::string> fillers;
        std::vector<std::string> variables;
        std::unordered_set<std::string> variables_set;

        std::string curr;
        for (const auto& v: content_) {
            if(v.first == SType::STRING) {
                curr += v.second;
            } else {
                variables.push_back(v.second);
                variables_set.insert(v.second);

                fillers.push_back(curr);
                curr = "";
            }
        }
        fillers.push_back(curr);

        if (variables_set != args.getVariableNameSet()) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Invalid template language string, arguments does not match."
            );
            return {};
        }

        return structure::TemplateLanguageString(
            *language,
            std::move(fillers),
            std::move(variables)
        );

    }

private:
    Identifier::Ptr language_type_;

    const std::vector<std::pair<SType, std::string>> content_;

};
}

