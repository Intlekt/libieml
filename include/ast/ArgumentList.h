#pragma once


#include "ast/interfaces/AST.h"
#include "structure/link/LinkArgumentDefinition.h"


#include <sstream>


namespace ieml::AST {

class ArgumentList : public virtual AST {
public:

    IEML_DECLARE_PTR_TYPE_AST(ArgumentList)

    ArgumentList(CharRange::Ptr&& char_range,
                 std::vector<Variable::Ptr>&& variables) : 
        AST(std::move(char_range)),
        variables_(std::move(variables)) {}

    virtual std::string to_string() const override {
        std::ostringstream os;
        os << "(";
        bool first = true;
        for (auto& v: variables_) {
            if (first) first = false;
            else       os << ", ";
            os << v->to_string();
        }
        os << ")";
        return os.str();
    };

    std::optional<structure::LinkArgumentDefinition> check_arguments(ieml::parser::ParserContextManager& ctx) const {
        // check no repetitions
        std::unordered_set<std::string> uniques;
        std::vector<std::string> variables;
        for (const auto& v: variables_) {
            uniques.insert(v->getName());
            variables.push_back(v->getName());
        }
        if (uniques.size() != variables.size()) {
            ctx.getErrorManager().visitorError(
                getCharRange(),
                "Cannot have repeated variable names in arguments definition."
            );
            return {};
        }

        return structure::LinkArgumentDefinition(variables);
    }

private:
    const std::vector<Variable::Ptr> variables_;
};

}