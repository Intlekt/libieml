#include "structure/Namespace.h"

namespace ieml::structure {


std::map<LanguageType, LanguageString> Name::build_traductions(std::unordered_set<LanguageString> traductions) {
    std::map<LanguageType, LanguageString> traductions_;
    for (const auto& item: traductions) {
        traductions_.insert({item.language(), item});
    }

    return traductions_;
}

}