#pragma once

#include <memory>
#include <string>

#include "structure/Path.h"
#include "structure/Constants.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class CategoryRegister {
public:
    void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase, DefinitionType type) {
        if (definition_types_.count(phrase) > 0) 
            throw std::invalid_argument("Category already defined.");
        
        definition_types_.insert({phrase, type});
        namespace_category_.define(name, phrase);
    }
    std::shared_ptr<structure::PathTree> resolve_category(const structure::LanguageString& s) const {
        return namespace_category_.resolve(s);
    }
    bool category_is_defined(const std::shared_ptr<structure::PathTree>& phrase) const {
        return namespace_category_.find(phrase) != namespace_category_.end();
    };

    std::shared_ptr<structure::Name> getName(const std::shared_ptr<structure::PathTree>& phrase) const {
        return namespace_category_.find(phrase)->second;
    };

    DefinitionType getDefinitionType(const std::shared_ptr<structure::PathTree>& phrase) const {
        return definition_types_.find(phrase)->second;
    }

    bool isNode(const std::shared_ptr<structure::PathTree>& phrase) const { 
        return getDefinitionType(phrase) == +DefinitionType::NODE;
    };

    typedef std::unordered_map<std::shared_ptr<structure::PathTree>, std::shared_ptr<Name>>::const_iterator  const_iterator;

    const_iterator categories_begin() const {return namespace_category_.begin();};
    const_iterator categories_end()   const {return namespace_category_.end();};

private:
    structure::Namespace<structure::PathTree> namespace_category_;
    std::unordered_map<std::shared_ptr<structure::PathTree>, DefinitionType> definition_types_;
};
}
