#pragma once

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IAuxiliary.h"
#include "ast/Auxiliary.h"

#include <sstream>


namespace ieml::AST {
class AuxiliaryParadigm : virtual public AST, public IAuxiliary {
public:
    AuxiliaryParadigm(CharRange::Ptr&& char_range,
                      std::vector<std::shared_ptr<IAuxiliary>>&& auxiliaries) : 
        AST(std::move(char_range)),
        auxiliaries_(std::move(auxiliaries)) {}
    

    std::string to_string() const override {
        std::ostringstream os;
        os << "{";
        bool first = true;
        for (auto& a: auxiliaries_)
            if (first) {
                os << a->to_string();
                first = false;
            } else 
                os << "; " << a->to_string();
        os << "}";
        return os.str();

    }

    virtual structure::PathNode::Vector check_auxiliary(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        structure::PathNode::Vector res;

        for (auto& auxiliary: auxiliaries_) {
            auto aux_set = auxiliary->check_auxiliary(ctx, role_type);

            if (aux_set.size() != 1) {
                ctx.getErrorManager().visitorError(
                    auxiliary->getCharRange(),
                    "An auxiliary paradigm can only be composed of singular auxiliaries."
                );
                return {nullptr};
            }

            if (!*aux_set.begin()) {
                return {nullptr};
            }

            res.push_back(*aux_set.begin());
        }

        return res;
    }

private:
    const std::vector<std::shared_ptr<IAuxiliary>> auxiliaries_;

};

}