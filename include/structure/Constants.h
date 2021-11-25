#pragma once

#include <enum.h>


namespace ieml::structure {

enum class PathType {
    ROOT,
    ROLE,
    JUNCTION_AUXILIARY,
    JUNCTION_AUXILIARY_INDEX,
    AUXILIARY,
    JUNCTION_INFLEXING,
    JUNCTION_INFLEXING_INDEX,
    INFLEXING,
    JUNCTION_CATEGORY,
    JUNCTION_CATEGORY_INDEX,
    WORD
};

BETTER_ENUM(JunctionType, char, AND = 0, OR);
BETTER_ENUM(InflexingType, char, PLURAL, SINGULAR);
BETTER_ENUM(AuxiliaryType, char, ABOVE = 0, BELOW);


BETTER_ENUM(LanguageType, char, FR, EN);

// enum JunctionType {
//     AND = 0
// };
// enum ReferenceType {};

// enum LanguageType {FR = 0, EN};

// const std::map<std::string, LanguageType> languageStr_to_LanguageType = {{"fr", LanguageType(FR)}, {"en", LanguageType(EN)}};
// const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};

enum RoleType {
    ROOT        = 0, 
    
    INITIATOR   = 1,
    INTERACTING = 2,
    RECIPIENT   = 3,

    CAUSE       = 4,
    TIME        = 5,
    LOCATION    = 6,
    INTENTION   = 7,
    MANNER      = 8
};

}


BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::LanguageType);