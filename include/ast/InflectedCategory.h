#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IReferencable.h"
#include "ast/interfaces/IInflectionList.h"
#include "ast/Reference.h"
#include "ast/Identifier.h"

#include "structure/Word.h"
#include "utils.h"


namespace ieml::AST {

class InflectedCategory : virtual public AST, public IReferencable {
public:
    InflectedCategory(std::shared_ptr<CharRange>&& char_range,
                      std::shared_ptr<IInflectionList>&& inflections,
                      std::shared_ptr<ICategory>&& category,
                      std::vector<std::shared_ptr<Reference>>&& references) :
        AST(std::move(char_range)),
        inflection_list_(std::move(inflections)),
        category_(std::move(category)),
        IReferencable(std::move(references)) {}

    std::string to_string() const override {
        std::ostringstream os;
        if (inflection_list_)
            os << inflection_list_->to_string() << " ";
        
        os << "#" << category_->to_string();
        os << references_to_string();

        return os.str();
    }

    structure::PathTree::Set check_flexed_category(parser::ParserContextManager& ctx, structure::RoleType role_type) const {
        auto category_set = category_->check_category(ctx);
        if (inflection_list_ != nullptr) {
            auto inflection_nodes = inflection_list_->check_inflection_list(ctx, role_type);

            if (!*category_set.begin() || !*inflection_nodes.begin())
                return {nullptr};

            return ctx.getPathTreeRegister().get_or_create_product(
                inflection_nodes,
                {category_set}
            );
        } else 
            return category_set;
    }

private:
    std::shared_ptr<IInflectionList> inflection_list_;
    std::shared_ptr<ICategory> category_;
};



}