#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IReferencable.h"
#include "ast/Reference.h"
#include "ast/Identifier.h"

#include "structure/Word.h"
#include "utils.h"


namespace ieml::AST {

class InflexedCategory : virtual public AST, public IReferencable {
public:
    InflexedCategory(std::unique_ptr<CharRange>&& char_range,
                     std::vector<std::unique_ptr<Identifier>>&& inflexions,
                     std::unique_ptr<ICategory>&& category,
                     std::vector<std::unique_ptr<Reference>>&& references) :
        AST(std::move(char_range)),
        inflexions_(std::move(inflexions)),
        category_(std::move(category)),
        IReferencable(std::move(references)) {}

    std::string to_string() const override {
        std::ostringstream os;
        bool first = true;


        for (auto && inflexion: inflexions_) {
            if (first) first = false;
            else os << " ";

            os << "~" << inflexion->to_string();
        }

        if (!first) os << " ";
        
        os << "#" << category_->to_string();
        os << references_to_string();

        return os.str();
    }

    std::shared_ptr<structure::PathTree> check_flexed_category(parser::ParserContext& ctx) const {
        auto category = category_->check_category(ctx);

        if (inflexions_.size()) {
            std::set<std::shared_ptr<structure::InflexingWord>> inflexions;

            for (auto&& inflexion_id: inflexions_) {
                auto inflexion = ctx.resolve_inflexion(inflexion_id->getName());

                if (!inflexion) {
                    ctx.getErrorManager().visitorError(
                        inflexion_id->getCharRange(),
                        "Undefined inflexion identifier '" + inflexion_id->getName() + "'."
                    );
                    return nullptr;
                }

                inflexions.insert(inflexion);

            }
            
            if (!category)
                return nullptr;

            return std::make_shared<structure::PathTree>(
                std::make_shared<structure::InflexingPathNode>(inflexions),
                std::set<std::shared_ptr<structure::PathTree>>{category}
            );

        } else {
            if (!category)
                return nullptr;
            return category;
        }
        
    }

private:
    std::vector<std::unique_ptr<Identifier>> inflexions_;
    std::unique_ptr<ICategory> category_;
};



}