#pragma once

#include <string>
#include <memory>
#include <sstream>
#include <any>

#include "ast/CharRange.h"

namespace ieml::AST {

class AST {
public:
    AST(std::unique_ptr<CharRange>&& char_range) : char_range_(std::move(char_range)) {};
    virtual ~AST() = default;

    virtual std::string to_string() const = 0;

    const CharRange& getCharRange() const {return *char_range_;}

private:
    std::unique_ptr<const CharRange> char_range_;

};

}
