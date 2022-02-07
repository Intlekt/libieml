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
#include "ast/PartialPathTree.h"

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
        IReferencable(std::move(references)),
        inflection_list_(std::move(inflections)),
        category_(std::move(category)) {}

    std::string to_string() const override {
        std::ostringstream os;
        if (inflection_list_)
            os << inflection_list_->to_string() << " ";
        
        os << "#" << category_->to_string();
        os << references_to_string();

        return os.str();
    }

    PartialPathTree::Optional check_flexed_category(parser::ParserContextManager& ctx, structure::RoleType role_type) const {
        auto references = check_references(ctx);

        auto category_set = category_->check_category(ctx);


        if (inflection_list_ != nullptr) {
            auto inflection_nodes = inflection_list_->check_inflection_list(ctx, role_type);

            if (!category_set || !references || !*inflection_nodes.begin())
                return {};

            return PartialPathTree::concat({std::move(category_set), std::move(references)})
                        ->prepend_node_product(ctx.getPathTreeRegister(), inflection_nodes);

        } else {
            if (!category_set || !references)
                return {};
            return PartialPathTree::concat({std::move(category_set), std::move(references)});
        }
    }

private:
    std::shared_ptr<IInflectionList> inflection_list_;
    std::shared_ptr<ICategory> category_;
};



}