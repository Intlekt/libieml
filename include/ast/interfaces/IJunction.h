#pragma once

#include <memory>

#include "ast/Constants.h"
#include "ast/interfaces/AST.h"
#include "structure/path/PathTree.h"
#include "ParserContextManager.h"


namespace ieml {
namespace AST {

struct Empty {};

class IJunction: public virtual AST {
public:
    virtual std::shared_ptr<structure::JunctionWord> check_junction(parser::ParserContextManager& ctx) const = 0;
};


template<class T, class IndexPathNode, class JunctionPathNode, class CheckArgument >
class IJunctionList {
public:
    IJunctionList(std::vector<std::shared_ptr<T>>&& items,
                  std::shared_ptr<IJunction>&& junction_identifier) : 
        items_(std::move(items)),
        junction_identifier_(std::move(junction_identifier)) {
            static_assert(std::is_base_of<AST, T>::value, "T is not an AST.");
        };

    virtual structure::PathTree::Vector check_junction_item(parser::ParserContextManager& ctx, size_t i, CheckArgument arg) const = 0;

    structure::PathTree::Vector check_junction(parser::ParserContextManager& ctx, CheckArgument arg) const {

        std::vector<structure::PathTree::Vector> children_list;

        bool valid = true;
        for (size_t i = 0; i < items_.size(); ++i) {

            auto phrase_set = check_junction_item(ctx, i, arg);

            structure::PathTree::Vector jonction_index_list;
            for (auto& phrase: phrase_set) {
                if (phrase == nullptr) {
                    valid = false;
                    continue;
                }
                jonction_index_list.push_back(ctx.getPathTreeRegister().get_or_create(std::make_shared<IndexPathNode>(i), {phrase}));
            }

            children_list.push_back(jonction_index_list);
        }

        const auto& junction_type = junction_identifier_->check_junction(ctx);

        if (junction_type == nullptr) {
            return {nullptr};
        }

        if (!valid) 
            return {nullptr};
        
        return ctx.getPathTreeRegister().get_or_create_product(std::make_shared<JunctionPathNode>(junction_type), children_list);
    };

protected:

    std::string junction_to_string() const {
        std::ostringstream os;

        os << junction_identifier_->to_string() << " [";

        bool first = true;
        for (auto&& item : items_) {
            if (first) first = false;
            else os << " ";

            os << item->to_string();
        }

        os << "]";
        return os.str();
    }
    
    std::vector<std::shared_ptr<T>> items_;
    std::shared_ptr<IJunction> junction_identifier_;
};

}
}