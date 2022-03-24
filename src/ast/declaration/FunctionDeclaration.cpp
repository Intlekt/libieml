#include "ast/declaration/FunctionDeclaration.h"


using namespace ieml::AST;


void FunctionDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    // check that the :
    //  - arguments
    //  - 


    // check link exists
    const auto link_partial = link_->check_category(ctx);
    if (!link_partial)
        return;

    if (link_partial->getPathTrees().size() != 1) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Link cannot be a paradigm identifier for a function definition."
        );
        return;
    }

    const auto link = *link_partial->getPathTrees().begin();
    if (!ctx.getLinkRegister().exists(link)) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Must provide a link identifier for a function definition."
        );
        return;
    }

    const auto link_def = ctx.getLinkRegister().getLinkByPathTree(link);
    
    // must check that the link definition allow the function type
    if (!link_def->canDescribeWord()) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "The link identifier is not suitable for a word function definition."
        );
        return;
    }

    auto domain = domain_->check_domain(ctx, link_def);
    if (!domain) return;


    auto condition = condition_->check_condition(ctx, link_def, *domain);
    if (!condition) return;

    if ((*condition)->getVariableNameSet() != link_def->getArguments().getVariableNameSet()) {
        ctx.getErrorManager().visitorError(
            getCharRange(),
            "Missing argument condition on some variables in word function definition."
        );
        return;
    }

    // test function not already exists ?
    
    ctx.getLinkRegister().register_function(link_def, std::move(*domain), std::move(*condition));
}
