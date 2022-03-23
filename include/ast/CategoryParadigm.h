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
    CategoryParadigm(CharRange::Ptr&& char_range, 
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
                os << "; " << c->to_string();
        os << "}";
        return os.str();
    }   
    virtual PartialPathTree::Optional check_category(parser::ParserContextManager& ctx) const override {
        std::vector<PartialPathTree::Optional> children_paradigm;

        bool valid = true;
        for (size_t i = 0; i < categories_.size(); i++) {
            auto child = categories_[i]->check_category(ctx);
            if (!child) {
                valid = false;
                continue;
            }
            children_paradigm.push_back(child);
        }

        if (!valid)
            return {};

        return PartialPathTree::concat(std::move(children_paradigm));
    };


private:
    std::vector<std::shared_ptr<ICategory>> categories_;
};
}
}