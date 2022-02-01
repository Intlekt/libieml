#include "ast/DimensionDefinition.h"


using namespace ieml::AST;


std::string DimensionDefinition::to_string() const {
    std::stringstream os;
    os << dimension_index_ << "d: ";

    bool first = true;
    for (auto&& path: paths_) {
        if (first) first = false;
        else       os << ";";
        os << path->to_string();
    }

    return os.str();
}

ieml::structure::PathTree::Set DimensionDefinition::check_dimension_definitions(ieml::parser::ParserContextManager& ctx, 
                                                                                const std::shared_ptr<ieml::structure::PathTree>& paradigm) const {
    bool valid = true;
    ieml::structure::PathTree::Set res;
    for (auto& path: paths_) {
        const auto& pt_path = path->check_path(ctx);

        if (!pt_path->is_contained(paradigm)) {
            ctx.getErrorManager().visitorError(
                getCharRange(), 
                "Invalid path for paradigm dimension definition, the path is not defined in the paradigm."
            );
            valid = false;
        } else {
            res.insert(pt_path);
        }
    }

    if (!valid) {
        return {};
    }

    return res;
}
