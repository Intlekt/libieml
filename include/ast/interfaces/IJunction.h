#pragma once

#include <memory>

#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/interfaces/AST.h"
#include "structure/path/PathTree.h"


namespace ieml {
namespace AST {

struct Empty {};

template<class T, class IndexPathNode, class JunctionPathNode, class CheckArgument >
class IJunction {
public:
    IJunction(std::vector<std::shared_ptr<T>>&& items,
              std::shared_ptr<Identifier>&& junction_identifier) : 
        items_(std::move(items)),
        junction_identifier_(std::move(junction_identifier)) {
            static_assert(std::is_base_of<AST, T>::value, "T is not an AST.");
        };

    virtual structure::PathTree::Set check_junction_item(parser::ParserContextManager& ctx, size_t i, CheckArgument arg) const = 0;

    structure::PathTree::Set check_junction(parser::ParserContextManager& ctx, CheckArgument arg) const {

        std::vector<structure::PathTree::Set> children_list;

        bool valid = true;
        for (size_t i = 0; i < items_.size(); ++i) {

            auto phrase_set = check_junction_item(ctx, i, arg);

            structure::PathTree::Set jonction_index_list;
            for (auto& phrase: phrase_set) {
                if (phrase == nullptr) {
                    valid = false;
                    continue;
                }
                jonction_index_list.insert(ctx.getPathTreeRegister().get_or_create(std::make_shared<IndexPathNode>(i), {phrase}));
            }

            children_list.push_back(jonction_index_list);
        }

        auto junction_type = ctx.getWordRegister().resolve_junction(structure::LanguageString(ctx.getLanguage(), junction_identifier_->getName()));

        if (junction_type == nullptr) {
            ctx.getErrorManager().visitorError(
                junction_identifier_->getCharRange(),
                "Undefined junction identifer '" + junction_identifier_->getName() + "'."
            );
            return {nullptr};
        }

        if (!valid) 
            return {nullptr};
        
        return ctx.getPathTreeRegister().get_or_create_product(std::make_shared<JunctionPathNode>(junction_type), children_list);


        // return ctx.getPathTreeRegister().get_or_create(std::make_shared<structure::RootPathNode>(), {junction});
    };

protected:

    std::string junction_to_string() const {
        std::ostringstream os;

        os << "&" << junction_identifier_->getName() << " [";

        bool first = true;
        for (auto&& item : items_) {
            if (first) first = false;
            else os << " ";

            os << item->to_string();
        }

        os << "]";
        return os.str();
    }

    std::shared_ptr<Identifier> junction_identifier_;
    std::vector<std::shared_ptr<T>> items_;
};

}
}