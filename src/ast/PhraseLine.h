#pragma once

#include <memory>
#include <sstream>

#include "ast/Constants.h"
#include "ast/AuxiliarySubPhraseLine.h"

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"

namespace ieml::AST {

class PhraseLine : virtual public AST {
public:
    PhraseLine(RoleType role_type,
               bool accentuation) : 
        role_type_(role_type),
        accentuation_(accentuation) {}

    RoleType getRoleType() const {return role_type_;}
    bool getAccentuation() const {return accentuation_;}

protected:
    std::string phrase_line_to_string() const {
        std::ostringstream os;

        os << std::to_string(role_type_) << " ";
        if (accentuation_)
            os << "! ";

        return os.str();
    }

private:
    const RoleType role_type_;
    const bool accentuation_;
};

class SimplePhraseLine : public PhraseLine {
public:
    SimplePhraseLine(std::unique_ptr<CharRange>&& char_range,
                     RoleType role_type,
                     bool accentuation,
                     std::unique_ptr<AuxiliarySubPhraseLine>&& auxiliary_subline) : 
        AST(std::move(char_range)),
        PhraseLine(role_type, accentuation),
        auxiliary_subline_(std::move(auxiliary_subline)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + auxiliary_subline_->to_string();
    }

private:
    std::unique_ptr<AuxiliarySubPhraseLine> auxiliary_subline_;

};

class JunctionPhraseLine : public PhraseLine, public IJunction<AuxiliarySubPhraseLine> {
public:
    JunctionPhraseLine(std::unique_ptr<CharRange>&& char_range,
                       std::vector<std::unique_ptr<AuxiliarySubPhraseLine>>&& sub_phrases,
                       std::unique_ptr<Identifier>&& junction_identifier,
                       RoleType role_type,
                       bool accentuation) : 
        AST(std::move(char_range)),
        PhraseLine(role_type, accentuation),
        IJunction(std::move(sub_phrases), std::move(junction_identifier)) {}

    std::string to_string() const override {
        return phrase_line_to_string() + junction_to_string();
    }

};

}