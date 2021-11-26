#pragma once

#include <map>
#include <memory>


#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"
#include "ast/interfaces/ICategory.h"

#include "ast/PhraseLine.h"

#include "ParserContext.h"
#include "structure/Phrase.h"
#include "structure/Path.h"


namespace ieml {
namespace AST {
class Phrase: virtual public AST, public ICategory, public IReferenceValue {
public:
    Phrase() : ICategory(), IReferenceValue() {}

    virtual std::shared_ptr<structure::Phrase> check_phrase(parser::ParserContext& ctx) const = 0;

    virtual std::shared_ptr<structure::PathTree> check_category(parser::ParserContext& ctx) const {
        return check_phrase(ctx);
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

    virtual std::shared_ptr<structure::Phrase> check_phrase(parser::ParserContext& ctx) const override {
        std::unordered_set<structure::RoleType> seen_nodes;
        
        std::vector<std::shared_ptr<structure::PathTree>> children;

        for (const auto& line: phrase_lines_) {
            auto tree = line->check_phrase_line(ctx);

            if (!tree) {
                continue;
            }
            
            auto role_number = std::dynamic_pointer_cast<structure::RoleNumberPathNode>(tree->getNode());

            if (!role_number) {
                // should not occur, maybe raise an exception

                ctx.getErrorManager().visitorError(
                    line->getCharRange(),
                    "Invalid phrase line"
                );
            } else {
                if (seen_nodes.count(role_number->getRoleType()) > 0) {
                    ctx.getErrorManager().visitorError(
                        line->getCharRange(),
                        "Duplicated phrase line number."
                    );
                } else {
                    seen_nodes.insert(role_number->getRoleType());
                    children.push_back(tree);
                }
            }
        }
        
        return std::make_shared<structure::Phrase>(children);
    };


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
    std::shared_ptr<structure::Phrase> check_phrase(parser::ParserContext& ctx) const override {
        
        // return structure::Phrase();
        return nullptr;
    };

};
}
}