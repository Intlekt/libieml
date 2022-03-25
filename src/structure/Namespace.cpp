#include "structure/Namespace.h"


namespace ieml::structure {


std::unordered_multimap<LanguageType, LanguageString> Name::build_traductions(std::unordered_multiset<LanguageString> traductions) {
    std::unordered_multimap<LanguageType, LanguageString> traductions_;
    for (const auto& item: traductions) {
        traductions_.insert({item.language(), item});
    }

    return traductions_;
}

Name TemplateName::getNameForValuation(std::unordered_map<std::string, Name::Ptr> valuation) const {
    std::unordered_map<LanguageType, TemplateLanguageString::Valuation> l_val;
    for (const auto& it : valuation) {
        for (const auto& it_name: *it.second) {
            const auto name = it_name.second.value();
            if (l_val[it_name.first].count(it.first) == 0)
                l_val[it_name.first].insert({it.first, name});
        }
    }

    Name::Traductions trads;
    for (const auto& it: *this) {
        trads.insert(it.second.getLanguageString(l_val[it.first]));
    }
    return Name(trads);
}


std::unordered_multimap<LanguageType, TemplateLanguageString> TemplateName::build_traductions(std::unordered_multiset<TemplateLanguageString> traductions) {
    std::unordered_multimap<LanguageType, TemplateLanguageString> traductions_;
    for (const auto& item: traductions) {
        traductions_.insert({item.language(), item});
    }
    return traductions_;
}

}