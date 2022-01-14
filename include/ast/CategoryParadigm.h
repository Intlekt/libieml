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
    virtual structure::PathTree::Set check_category(parser::ParserContextManager& ctx) const override {
        structure::PathTree::Set children_paradigm;

        bool valid = true;
        for (size_t i = 0; i < categories_.size(); i++) {
            auto children = categories_[i]->check_category(ctx);
            for (auto& child: children) {
                if (!child) valid = false;
                children_paradigm.insert(child);
            }
        }

        if (!valid)
            return {nullptr};

        return children_paradigm;
    };


private:
    std::vector<std::shared_ptr<ICategory>> categories_;
};
}
}