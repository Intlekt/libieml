#pragma once

#include <memory>
#include <string>

#include "structure/Path.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class CategoryRegister {
public:
    virtual void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase, bool is_node) = 0;
    virtual std::shared_ptr<structure::PathTree> resolve_category(const std::string& s) const = 0;

    virtual bool category_is_defined(const std::shared_ptr<structure::PathTree>& phrase) const = 0;
    
    typedef std::unordered_map<std::shared_ptr<structure::PathTree>, std::shared_ptr<structure::Name>>::const_iterator category_const_iterator;

    virtual category_const_iterator categories_begin() const = 0;
    virtual category_const_iterator categories_end() const = 0;
};
}
