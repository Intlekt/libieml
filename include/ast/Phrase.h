#pragma once



#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"
#include "ast/interfaces/ICategory.h"

#include "ast/PhraseLine.h"

#include "ParserContext.h"
#include "structure/Phrase.h"


namespace ieml::AST {
class Phrase: virtual public AST, public ICategory, public IReferenceValue {
public:
    Phrase() : ICategory(), IReferenceValue() {}

    virtual structure::Phrase check_phrase(parser::ParserContext& ctx) const {
        return structure::Phrase();
    };

};


class SimplePhrase : public Phrase {
public:
    SimplePhrase(std::unique_ptr<CharRange> && char_range,
                 std::vector<std::unique_ptr<PhraseLine>> && phrase_lines) : 
            AST(std::move(char_range)),
            Phrase(), 
            phrase_lines_(std::move(phrase_lines)) {}

    std::string to_string() const override {
        std::ostringstream os;

        os << "(";

        bool first = true;
        for (auto&& line : phrase_lines_) {
            if (first) first = false;
            else os << ", ";

            os << line->to_string();
        }

        os << ")";
        return os.str();
    }

private:
    std::vector<std::unique_ptr<PhraseLine>> phrase_lines_;

};


class JunctionPhrase : public Phrase, public IJunction<Phrase> {
public:
    JunctionPhrase(std::unique_ptr<CharRange>&& char_range,
                   std::vector<std::unique_ptr<Phrase>>&& phrases,
                   std::unique_ptr<Identifier>&& junction_identifier) : 
        AST(std::move(char_range)),
        Phrase(), 
        IJunction<Phrase>(std::move(phrases), std::move(junction_identifier)) {}

    std::string to_string() const override {
        return "(" + junction_to_string() + ")";
    }
};

}