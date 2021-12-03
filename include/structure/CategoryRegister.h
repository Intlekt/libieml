#pragma once

#include <memory>
#include <string>

#include "structure/Path.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class CategoryRegister {
public:
    void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase, bool is_node) {
        if (category_is_node_.count(phrase) > 0) 
            throw std::invalid_argument("Phrase already defined.");
        
        category_is_node_.insert({phrase, is_node});
        namespace_category_.define(name, phrase);
    }
    std::shared_ptr<structure::PathTree> resolve_category(const structure::LanguageString& s) const {
        return namespace_category_.resolve(s);
    }
    bool category_is_defined(const std::shared_ptr<structure::PathTree>& phrase) const {
        return namespace_category_.find(phrase) != namespace_category_.end();
    };

    typedef std::unordered_map<std::shared_ptr<structure::PathTree>, std::shared_ptr<Name>>::const_iterator  const_iterator;

    const_iterator categories_begin() const {return namespace_category_.begin();};
    const_iterator categories_end()   const {return namespace_category_.end();};

private:
    structure::Namespace<structure::PathTree> namespace_category_;
    std::unordered_map<std::shared_ptr<structure::PathTree>, bool> category_is_node_;
};
}
