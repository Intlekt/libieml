#pragma once

#include <memory>
#include <set>

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IAuxiliary.h"

#include "ast/InflectedCategory.h"


namespace ieml::AST {

class AuxiliarySubPhraseLine : virtual public AST {
public:
    AuxiliarySubPhraseLine(std::shared_ptr<IAuxiliary>&& auxiliary) : 
        auxiliary_(std::move(auxiliary)) {}

    structure::PathTree::Vector check_auxiliary_subline(parser::ParserContextManager& ctx, structure::RoleType role_type) const {
        auto child_set = _check_auxiliary_subline(ctx, role_type);

        if (auxiliary_) {
            auto auxiliary_nodes = auxiliary_->check_auxiliary(ctx, role_type);

            if(!*child_set.begin() || !*auxiliary_nodes.begin()) {
                return {nullptr};
            }
            
            return ctx.getPathTreeRegister().get_or_create_product(auxiliary_nodes, {child_set});
        } else  
            return child_set;
        
    };  


protected:
    /**
     * @brief Check the rest of the auxiliary line except the auxiliary *... part.
     * 
     * @param ctx 
     * @param role_type 
     * @return structure::PathTree::Set 
     */
    virtual structure::PathTree::Vector _check_auxiliary_subline(parser::ParserContextManager& ctx, structure::RoleType role_type) const = 0;

    std::string auxiliary_to_string() const {
        if (auxiliary_)
            return auxiliary_->to_string() + " ";
        else
            return "";
    }

private:
    const std::shared_ptr<IAuxiliary> auxiliary_;
};

class SimpleAuxiliarySubPhraseLine : public AuxiliarySubPhraseLine {
public:
    SimpleAuxiliarySubPhraseLine(std::shared_ptr<CharRange>&& char_range,
                                 std::shared_ptr<IAuxiliary>&& auxiliary,
                                 std::shared_ptr<InflectedCategory>&& flexed_category) : 
        AST(std::move(char_range)),
        AuxiliarySubPhraseLine(std::move(auxiliary)),
        flexed_category_(std::move(flexed_category)) {}


    std::string to_string() const override {
        return auxiliary_to_string() + flexed_category_->to_string();
    }

protected:
    virtual structure::PathTree::Vector _check_auxiliary_subline(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        return flexed_category_->check_flexed_category(ctx, role_type);
    };


private:
    std::shared_ptr<InflectedCategory> flexed_category_;
};

class JunctionAuxiliarySubPhraseLine : 
    public AuxiliarySubPhraseLine, 
    public IJunctionList<InflectedCategory, structure::InflectionJunctionIndexPathNode, structure::InflectionJunctionPathNode, structure::RoleType> {
public:
    JunctionAuxiliarySubPhraseLine(std::shared_ptr<CharRange>&& char_range,
                                   std::shared_ptr<IAuxiliary>&& auxiliary,
                                   std::vector<std::shared_ptr<InflectedCategory>>&& flexed_categories,
                                   std::shared_ptr<IJunction>&& junction_type) :
        AST(std::move(char_range)),
        AuxiliarySubPhraseLine(std::move(auxiliary)),
        IJunctionList<InflectedCategory, structure::InflectionJunctionIndexPathNode, structure::InflectionJunctionPathNode, structure::RoleType>(std::move(flexed_categories), std::move(junction_type)) {}

    std::string to_string() const override {
        return auxiliary_to_string() + junction_to_string();
    }

protected:
    virtual structure::PathTree::Vector _check_auxiliary_subline(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        return check_junction(ctx, role_type);
    };

    virtual structure::PathTree::Vector check_junction_item(parser::ParserContextManager& ctx, size_t i, structure::RoleType role_type) const override {
        return items_[i]->check_flexed_category(ctx, role_type);
    };
};

}