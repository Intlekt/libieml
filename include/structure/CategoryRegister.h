#pragma once

#include <memory>
#include <string>

#include "structure/path/PathTree.h"
#include "structure/Constants.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class CategoryRegister {
public:
    CategoryRegister() = default;

    CategoryRegister(const CategoryRegister&) = delete;
    CategoryRegister& operator=(CategoryRegister&) = delete;

    void define_link(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase) {
        define_category(name, phrase, DefinitionType::LINK);
    }

    void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase, DefinitionType type) {
        if (definition_types_.count(phrase) > 0) 
            throw std::invalid_argument("Category already defined.");
        
        definition_types_.insert({phrase, type});
        namespace_category_.define(name, phrase);

        if (phrase->is_phrase_word())
            phrase_words_.insert({phrase->get_phrase_word(), phrase});
    }
    typedef std::unordered_multimap<Word::Ptr, PathTree::Ptr> PhraseWordMap;

    std::pair<PhraseWordMap::const_iterator, PhraseWordMap::const_iterator> get_phrase_words(const Word::Ptr& word) const {
        return phrase_words_.equal_range(word);
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

    PhraseWordMap phrase_words_;
};
}
