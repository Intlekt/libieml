#pragma once

#include "ast/interfaces/AST.h"
#include "ast/Path.h"

#include <sstream>
#include <memory>
#include <vector>


namespace ieml::AST {
class DimensionDefinition: public virtual AST {
public:
    DimensionDefinition(std::shared_ptr<CharRange>&& char_range,
                        size_t dimension_index,
                        std::vector<std::shared_ptr<Path>>&& paths) :
        AST(std::move(char_range)),
        dimension_index_(dimension_index),
        paths_(std::move(paths)) {}

    virtual std::string to_string() const override {
        std::stringstream os;
        os << dimension_index_ << "d: ";

        bool first = true;
        for (auto&& path: paths_) {
            if (first) first = false;
            else       os << ";";
            os << path->to_string();
        }

        return os.str();
    }

private:
    const size_t dimension_index_;
    const std::vector<std::shared_ptr<Path>> paths_;
};
}