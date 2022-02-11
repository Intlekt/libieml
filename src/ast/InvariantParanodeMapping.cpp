#include "ast/InvariantParanodeMapping.h"


using namespace ieml::AST;


InvariantParanodeMapping::CheckResult InvariantParanodeMapping::check_mapping(ieml::parser::ParserContextManager& ctx) const {

    auto invariant_partial = invariant_->check_category(ctx);
    auto paranode = paranode_->check_paradigm(ctx);

    if (!invariant_partial || !paranode) {
        return {};
    }

    auto invariant_v = invariant_partial->getPathTrees();
    if (invariant_v.size() != 1) {
        ctx.getErrorManager().visitorError(
            invariant_->getCharRange(), 
            "Expects a node for the invariant identifier."
        );
        return {};
    }
    auto invariant = invariant_v[0];

    auto pt_register = ctx.getPathTreeRegister();

    if (pt_register.invariant_paths(*paranode) != pt_register.paths(invariant)) {
        ctx.getErrorManager().visitorError(
            getCharRange(), 
            "Invalid invariant \"" + invariant_->to_string() + "\" for paranode \"" + paranode_->getName() + "\"."
        );
        return {};
    }

    return std::pair{invariant, *paranode};
}   
