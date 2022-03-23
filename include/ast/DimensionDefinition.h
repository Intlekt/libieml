#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Path.h"

#include <sstream>
#include <memory>
#include <vector>


namespace ieml::AST {
class DimensionDefinition: public virtual AST {
public:
    DimensionDefinition(CharRange::Ptr&& char_range,
                        size_t dimension_index,
                        std::vector<std::shared_ptr<Path>>&& paths) :
        AST(std::move(char_range)),
        dimension_index_(dimension_index),
        paths_(std::move(paths)) {}

    virtual std::string to_string() const override;

    size_t getDimensionIndex() const {return dimension_index_;};

    ieml::structure::PathTree::Set check_dimension_definitions(ieml::parser::ParserContextManager& ctx, const std::shared_ptr<ieml::structure::PathTree>& paradigm) const;

private:
    const size_t dimension_index_;
    const std::vector<std::shared_ptr<Path>> paths_;
};
}