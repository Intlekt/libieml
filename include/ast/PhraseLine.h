#pragma once

#include <memory>
#include <sstream>
#include <set>

#include "ast/Constants.h"
#include "ast/RoleType.h"
#include "ast/AuxiliarySubPhraseLine.h"

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"

#include "structure/path/PathNode.h"
#include "ParserContextManager.h"


namespace ieml::AST {

class PhraseLine : virtual public AST {
public:
    PhraseLine(std::shared_ptr<RoleType>&& role_type,
               bool accentuation) : 
        role_type_(std::move(role_type)),
        accentuation_(accentuation) {}

    std::shared_ptr<RoleType> getRoleType() const {return role_type_;}
    bool getAccentuation() const {return accentuation_;}

    PartialPathTree::Optional check_phrase_line(parser::ParserContextManager& ctx) const {
        auto type = role_type_->check_role_type(ctx);
        auto child_set = _check_phrase_line(ctx, *type);

        if (!child_set || !type) {
            return {};
        }

        return child_set->prepend_node(ctx.getPathTreeRegister(), std::make_shared<structure::RoleNumberPathNode>(*type));
    };
    

protected:
    virtual PartialPathTree::Optional _check_phrase_line(parser::ParserContextManager& ctx, structure::RoleType role_type) const = 0;


    std::string phrase_line_to_string() const {
        std::ostringstream os;

        os << role_type_->to_string() << " ";
        if (accentuation_)
            os << "! ";

        return os.str();
    }

private:
    const std::shared_ptr<RoleType> role_type_;
    const bool accentuation_;
};

class SimplePhraseLine : public PhraseLine {
public:
    SimplePhraseLine(std::shared_ptr<CharRange>&& char_range,
                     std::shared_ptr<RoleType>&& role_type,
                     bool accentuation,
                     std::shared_ptr<AuxiliarySubPhraseLine>&& auxiliary_subline) : 
        AST(std::move(char_range)),
        PhraseLine(std::move(role_type), accentuation),
        auxiliary_subline_(std::move(auxiliary_subline)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + auxiliary_subline_->to_string();
    }
protected:
    PartialPathTree::Optional _check_phrase_line(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        return auxiliary_subline_->check_auxiliary_subline(ctx, role_type);
    };

private:
    std::shared_ptr<AuxiliarySubPhraseLine> auxiliary_subline_;

};

class JunctionPhraseLine : public PhraseLine, public IJunctionList<AuxiliarySubPhraseLine, structure::AuxiliaryJunctionIndexPathNode, structure::AuxiliaryJunctionPathNode, structure::RoleType> {
public:
    JunctionPhraseLine(std::shared_ptr<CharRange>&& char_range,
                       std::vector<std::shared_ptr<AuxiliarySubPhraseLine>>&& sub_phrases,
                       std::shared_ptr<IJunction>&& junction_identifier,
                       std::shared_ptr<RoleType>&& role_type,
                       bool accentuation) : 
        AST(std::move(char_range)),
        PhraseLine(std::move(role_type), accentuation),
        IJunctionList<AuxiliarySubPhraseLine, structure::AuxiliaryJunctionIndexPathNode, structure::AuxiliaryJunctionPathNode, structure::RoleType>(std::move(sub_phrases), std::move(junction_identifier)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + junction_to_string();
    }

protected:
    virtual PartialPathTree::Optional check_junction_item(parser::ParserContextManager& ctx, size_t i, structure::RoleType role_type) const override {
        return items_[i]->check_auxiliary_subline(ctx, role_type);
    };

    virtual PartialPathTree::Optional _check_phrase_line(parser::ParserContextManager& ctx, structure::RoleType role_type) const override {
        return check_junction(ctx, role_type);
    }
};

}