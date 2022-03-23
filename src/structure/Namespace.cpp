#include "structure/Namespace.h"

namespace ieml::structure {


std::unordered_multimap<LanguageType, LanguageString> Name::build_traductions(std::unordered_multiset<LanguageString> traductions) {
    std::unordered_multimap<LanguageType, LanguageString> traductions_;
    for (const auto& item: traductions) {
        traductions_.insert({item.language(), item});
    }

    return traductions_;
}

std::unordered_multimap<LanguageType, TemplateLanguageString> TemplateName::build_traductions(std::unordered_multiset<TemplateLanguageString> traductions) {
    std::unordered_multimap<LanguageType, TemplateLanguageString> traductions_;
    for (const auto& item: traductions) {
        traductions_.insert({item.language(), item});
    }

    return traductions_;
}

}