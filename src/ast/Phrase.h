#pragma once



#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"

#include "ast/PhraseLine.h"


namespace ieml::AST {
class Phrase: public AST {
public:
    Phrase(std::unique_ptr<CharRange> && char_range) : AST(std::move(char_range)) {}

};


class SimplePhrase : public Phrase {
public:
    SimplePhrase(std::unique_ptr<CharRange> && char_range,
                 std::vector<std::unique_ptr<PhraseLine>> && phrase_lines) : 
            Phrase(std::move(char_range)), 
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


class JunctionPhrase : public Phrase, public IJunction {
public:
    JunctionPhrase(std::unique_ptr<CharRange> && char_range,
                   JunctionType junction_type) : 
        Phrase(std::move(char_range)),
        IJunction(junction_type) {}

};

}