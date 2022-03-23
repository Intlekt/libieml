#pragma once

#include <map>
#include <memory>
#include <set>


#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"
#include "ast/interfaces/ICategory.h"

#include "ast/PhraseLine.h"

#include "ParserContextManager.h"
#include "structure/path/PathNode.h"


namespace ieml {
namespace AST {
class Phrase: virtual public AST, public ICategory, public IReferenceValue {
public:
    Phrase() : ICategory(), IReferenceValue() {}

    virtual PartialPathTree::Optional check_phrase(parser::ParserContextManager& ctx) const = 0;

    virtual PartialPathTree::Optional check_category(parser::ParserContextManager& ctx) const {
        return check_phrase(ctx);
    };

};


class SimplePhrase : public Phrase {
public:
    SimplePhrase(CharRange::Ptr && char_range,
                 std::vector<std::shared_ptr<PhraseLine>> && phrase_lines) : 
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

    virtual PartialPathTree::Optional check_phrase(parser::ParserContextManager& ctx) const override {
        std::unordered_set<structure::RoleType> seen_nodes;
        
        std::vector<PartialPathTree::Optional> children_list;
        children_list.reserve(phrase_lines_.size());

        bool valid = true;
        for (const auto& line: phrase_lines_) {
            auto phrase_line_set = line->check_phrase_line(ctx);

            if (!phrase_line_set) {
                valid = false;
                continue;
            }
            
            auto role_number = std::dynamic_pointer_cast<structure::RoleNumberPathNode>((*phrase_line_set->getPathTrees().begin())->getNode());

            if (!role_number) {
                // should not occur
                throw std::runtime_error("Not a role number path");
            } else {
                if (seen_nodes.count(role_number->getRoleType()) > 0) {
                    ctx.getErrorManager().visitorError(
                        line->getCharRange(),
                        "Duplicated phrase line number."
                    );
                    valid = false;
                } else {
                    seen_nodes.insert(role_number->getRoleType());
                    children_list.emplace_back(std::move(phrase_line_set));
                }
            }
        }

        if (!valid)
            return {};

        if (seen_nodes.find(structure::RoleType::ROOT) == seen_nodes.end()) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Missing root role."
            );
            return {};
        }

        return PartialPathTree::product(ctx.getPathTreeRegister(), 
                                        std::make_shared<structure::RootPathNode>(), 
                                        std::move(children_list));
    };


private:
    std::vector<std::shared_ptr<PhraseLine>> phrase_lines_;

};


class JunctionPhrase : public Phrase, public IJunctionList<Phrase, structure::PhraseJunctionIndexPathNode, structure::PhraseJunctionPathNode, Empty> {
public:
    JunctionPhrase(CharRange::Ptr&& char_range,
                   std::vector<std::shared_ptr<Phrase>>&& phrases,
                   std::shared_ptr<IJunction>&& junction_identifier) : 
        AST(std::move(char_range)),
        Phrase(), 
        IJunctionList<Phrase, structure::PhraseJunctionIndexPathNode, structure::PhraseJunctionPathNode, Empty>(std::move(phrases), std::move(junction_identifier)) {}

    std::string to_string() const override {
        return "(" + junction_to_string() + ")";
    }
    
    virtual PartialPathTree::Optional check_junction_item(parser::ParserContextManager& ctx, size_t i, 
                                                            __attribute__((unused)) Empty a) const override {
        return items_[i]->check_phrase(ctx);
    };

    virtual PartialPathTree::Optional check_phrase(parser::ParserContextManager& ctx) const override {
        return check_junction(ctx, {});
    }
};
}
}