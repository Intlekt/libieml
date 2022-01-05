#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/ICategory.h"


namespace ieml {
namespace AST {

class CategoryParadigm: public virtual AST, public ICategory {
public:
    CategoryParadigm(std::shared_ptr<CharRange>&& char_range, 
                     std::vector<std::shared_ptr<ICategory>> categories) : 
        AST(std::move(char_range)),
        categories_(categories) {}

    virtual std::string to_string() const override {
        std::stringstream os;
        os << "{";
        bool first = true;
        for (auto& c: categories_)
            if (first) {
                os << c->to_string();
                first = false;
            } else 
                os << ", " << c->to_string();
        os << "}";
        return os.str();
    }   
    virtual std::shared_ptr<structure::PathTree> check_category(parser::ParserContextManager& ctx) const override {
        structure::PathTree::Children children;
        for (size_t i = 0; i < categories_.size(); i++) {
            auto child = categories_[i]->check_category(ctx);
            if (child) {
                auto child_ = ctx.getPathTreeRegister().get_or_create(std::make_shared<structure::ParadigmIndexPathNode>(i), {child});
                children.insert(child_);
            }
        }

        return ctx.getPathTreeRegister().get_or_create(std::make_shared<structure::ParadigmPathNode>(), children);
    };


private:
    std::vector<std::shared_ptr<ICategory>> categories_;
};
}
}