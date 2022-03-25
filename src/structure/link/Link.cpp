#include "structure/link/Link.h"
#include "ParserContextManager.h"

using namespace ieml::structure;


const Name Link::getNameForValuation(PathTree::Register& preg, CategoryRegister& creg, WordRegister& wreg, const WordValuation& valuation) const {
    std::unordered_map<std::string, ieml::structure::Name::Ptr> template_v;
    for (const auto& it : valuation) {
        Name::Ptr name;
        if (wreg.get_script_type(it.second->getScript()) == +WordType::CATEGORY) {
            // resolve phrase word
            const auto phrase_word = preg.build_phrase_word(std::dynamic_pointer_cast<CategoryWord>(it.second), phrase_tree_inflection_);

            if (creg.category_is_defined(phrase_word)) 
                name = creg.getName(phrase_word);
        } else {
            name = wreg.getName(it.second);
        }

        if (!name) {
            // no name
            Name::Traductions ls;
            name = std::make_shared<Name>(ls);
        }

        template_v.insert({it.first, name});
    }

    return template_language_string_.getNameForValuation(template_v);
}
