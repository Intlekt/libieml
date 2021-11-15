#pragma once

#include <memory>
#include <sstream>

#include "ast/Constants.h"
#include "ast/AuxiliarySubPhraseLine.h"

#include "ast/interfaces/AST.h"
#include "ast/interfaces/IJunction.h"

namespace ieml::AST {

class PhraseLine : public AST {
public:
    PhraseLine(std::unique_ptr<CharRange>&& char_range,
               RoleType role_type,
               bool accentuation) : 
        AST(std::move(char_range)),
        role_type_(role_type),
        accentuation_(accentuation) {}

    RoleType getRoleType() const {return role_type_;}
    bool getAccentuation() const {return accentuation_;}

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
        PhraseLine(std::move(char_range), role_type, accentuation),
        auxiliary_subline_(std::move(auxiliary_subline)) {}

    std::string to_string() const override {
        std::ostringstream os;

        os << std::to_string(getRoleType()) << " ";
        if (getAccentuation())
            os << "! ";
        
        os << auxiliary_subline_->to_string();

        return os.str();
    }

private:
    std::unique_ptr<AuxiliarySubPhraseLine> auxiliary_subline_;

};

class JunctionPhraseLine : public PhraseLine, public IJunction {

};

}