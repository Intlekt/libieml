#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <any>

#include "ast/CharRange.h"

#include "ast/macro_utils.h"

namespace ieml::AST {

class AST {
public:
    IEML_DECLARE_PTR_TYPE_AST(AST)

    AST(CharRange::Ptr&& char_range) : char_range_(std::move(char_range)) {};
    virtual ~AST() = default;

    virtual std::string to_string() const = 0;

    const CharRange& getCharRange() const {return *char_range_;}

private:
    const CharRange::Ptr char_range_;
};

}
