#pragma once

#include <memory>
#include <sstream>
#include <set>

#include "ast/Constants.h"
#include "ast/AuxiliarySubPhraseLine.h"

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"

#include "structure/Path.h"
#include "ParserContext.h"


namespace ieml::AST {

class PhraseLine : virtual public AST {
public:
    PhraseLine(int role_type,
               bool accentuation) : 
        role_type_(role_type),
        accentuation_(accentuation) {}

    int getRoleType() const {return role_type_;}
    bool getAccentuation() const {return accentuation_;}

    std::shared_ptr<structure::PathTree> check_phrase_line(parser::ParserContext& ctx) const {
        std::shared_ptr<structure::PathTree> child = _check_phrase_line(ctx);

        auto type = structure::RoleType::_from_integral_nothrow(role_type_);
        if (!type) {
            ctx.getErrorManager().visitorError(
                getCharRange(), "Invalid role number, got '" + std::to_string(role_type_) + "'"
            );
        }

        if (!child || !type) {
            return nullptr;
        }

        return ctx.getPathTreeRegister().get_or_create(
            std::make_shared<structure::RoleNumberPathNode>(*type), 
            structure::PathTree::Children{child}
        );
    };
    

protected:
    virtual std::shared_ptr<structure::PathTree> _check_phrase_line(parser::ParserContext& ctx) const = 0;


    std::string phrase_line_to_string() const {
        std::ostringstream os;

        os << std::to_string(role_type_) << " ";
        if (accentuation_)
            os << "! ";

        return os.str();
    }

private:
    const int role_type_;
    const bool accentuation_;
};

class SimplePhraseLine : public PhraseLine {
public:
    SimplePhraseLine(std::unique_ptr<CharRange>&& char_range,
                     int role_type,
                     bool accentuation,
                     std::unique_ptr<AuxiliarySubPhraseLine>&& auxiliary_subline) : 
        AST(std::move(char_range)),
        PhraseLine(role_type, accentuation),
        auxiliary_subline_(std::move(auxiliary_subline)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + auxiliary_subline_->to_string();
    }
protected:
    std::shared_ptr<structure::PathTree> _check_phrase_line(parser::ParserContext& ctx) const override {
        return auxiliary_subline_->check_auxiliary_subline(ctx);
    };

private:
    std::unique_ptr<AuxiliarySubPhraseLine> auxiliary_subline_;

};

class JunctionPhraseLine : public PhraseLine, public IJunction<AuxiliarySubPhraseLine> {
public:
    JunctionPhraseLine(std::unique_ptr<CharRange>&& char_range,
                       std::vector<std::unique_ptr<AuxiliarySubPhraseLine>>&& sub_phrases,
                       std::unique_ptr<Identifier>&& junction_identifier,
                       int role_type,
                       bool accentuation) : 
        AST(std::move(char_range)),
        PhraseLine(role_type, accentuation),
        IJunction(std::move(sub_phrases), std::move(junction_identifier)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + junction_to_string();
    }

protected:
    std::shared_ptr<structure::PathTree> _check_phrase_line(parser::ParserContext& ctx) const override {
        return nullptr;
    }
};

}