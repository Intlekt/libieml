#pragma once

#include <memory>
#include <set>

#include "ast/interfaces/AST.h"

#include "ast/InflexedCategory.h"


namespace ieml::AST {

class AuxiliarySubPhraseLine : virtual public AST {
public:
    AuxiliarySubPhraseLine(std::shared_ptr<Identifier>&& auxiliary) : 
        auxiliary_(std::move(auxiliary)) {}


    std::shared_ptr<structure::PathTree> check_auxiliary_subline(parser::ParserContext& ctx) const {
        auto child = _check_auxiliary_subline(ctx);

        if (auxiliary_) {
            auto auxiliary = ctx.getWordRegister().resolve_auxiliary(structure::LanguageString(ctx.getLanguage(), auxiliary_->getName()));
            if (!auxiliary) {
                ctx.getErrorManager().visitorError(
                    auxiliary_->getCharRange(),
                    "Undefined auxiliary identifier '" + auxiliary_->getName() + "'."
                );
                return nullptr;
            }
            if(!child) {
                return nullptr;
            }
            
            return ctx.getPathTreeRegister().get_or_create(
                std::make_shared<structure::AuxiliaryPathNode>(auxiliary), 
                structure::PathTree::Children{child});
        } else {
            if(!child) 
                return nullptr;
            
            return child;
        }
    };  


protected:
    virtual std::shared_ptr<structure::PathTree> _check_auxiliary_subline(parser::ParserContext& ctx) const = 0;

    std::string auxiliary_to_string() const {
        if (auxiliary_)
            return "*" + auxiliary_->to_string() + " ";
        else
            return "";
    }

private:
    std::shared_ptr<Identifier> auxiliary_;
};

class SimpleAuxiliarySubPhraseLine : public AuxiliarySubPhraseLine {
public:
    SimpleAuxiliarySubPhraseLine(std::shared_ptr<CharRange>&& char_range,
                                 std::shared_ptr<Identifier>&& auxiliary,
                                 std::shared_ptr<InflexedCategory>&& flexed_category) : 
        AST(std::move(char_range)),
        AuxiliarySubPhraseLine(std::move(auxiliary)),
        flexed_category_(std::move(flexed_category)) {}


    std::string to_string() const override {
        return auxiliary_to_string() + flexed_category_->to_string();
    }

protected:
    std::shared_ptr<structure::PathTree> _check_auxiliary_subline(parser::ParserContext& ctx) const override {
        return flexed_category_->check_flexed_category(ctx);
    };


private:
    std::shared_ptr<InflexedCategory> flexed_category_;
};

class JunctionAuxiliarySubPhraseLine : public AuxiliarySubPhraseLine, public IJunction<InflexedCategory, structure::InflectionJunctionIndexPathNode, structure::InflectionJunctionPathNode> {
public:
    JunctionAuxiliarySubPhraseLine(std::shared_ptr<CharRange>&& char_range,
                                   std::shared_ptr<Identifier>&& auxiliary,
                                   std::vector<std::shared_ptr<InflexedCategory>>&& flexed_categories,
                                   std::shared_ptr<Identifier>&& junction_type) :
        AST(std::move(char_range)),
        AuxiliarySubPhraseLine(std::move(auxiliary)),
        IJunction<InflexedCategory, structure::InflectionJunctionIndexPathNode, structure::InflectionJunctionPathNode>(std::move(flexed_categories), std::move(junction_type)) {}

    std::string to_string() const override {
        return auxiliary_to_string() + junction_to_string();
    }

protected:
    std::shared_ptr<structure::PathTree> _check_auxiliary_subline(parser::ParserContext& ctx) const override {
        return check_junction(ctx);
    };

    virtual std::shared_ptr<structure::PathTree> check_junction_item(parser::ParserContext& ctx, size_t i) const override {
        return items_[i]->check_flexed_category(ctx);
    };
};

}