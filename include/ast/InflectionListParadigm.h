#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IInflectionList.h"
#include "ast/InflectionList.h"

#include <sstream>

namespace ieml::AST {

class InflectionListParadigm: virtual public AST, public IInflectionList {
public:
    InflectionListParadigm(CharRange::Ptr&& char_range,
                           std::vector<std::shared_ptr<IInflectionList>>&& inflection_lists) : 
        AST(std::move(char_range)),
        inflection_lists_(std::move(inflection_lists)) {}

    virtual std::string to_string() const override {
        std::ostringstream os;
        os << "{";
        bool first = true;
        for (auto& c: inflection_lists_)
            if (first) {
                os << c->to_string();
                first = false;
            } else 
                os << "; " << c->to_string();
        os << "}";
        return os.str();
    }

    virtual structure::PathNode::Vector check_inflection_list(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        structure::PathNode::Vector res;
        for (auto& inflection_list: inflection_lists_) {
            auto pathnode_set = inflection_list->check_inflection_list(ctx, role_type);

            if (pathnode_set.size() != 1) {
                ctx.getErrorManager().visitorError(
                    inflection_list->getCharRange(),
                    "An inflection list paradigm can only be composed of singular inflection lists."
                );
                return {nullptr};
            }
            
            if (!*pathnode_set.begin())
                return {nullptr};

            res.push_back(*pathnode_set.begin());
        }

        return res;
    }

private:
    const std::vector<std::shared_ptr<IInflectionList>> inflection_lists_;

};
}