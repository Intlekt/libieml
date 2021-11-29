#pragma once

#include <enum.h>


namespace ieml::structure {

BETTER_ENUM(PathType, char,
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
);

BETTER_ENUM(JunctionType, char, AND = 1, OR);
// BETTER_ENUM(InflexingType, char, PLURAL = 1, SINGULAR);
// BETTER_ENUM(AuxiliaryType, char, ABOVE = 1, BELOW);


BETTER_ENUM(LanguageType, char, FR = 1, EN);

// enum JunctionType {
//     AND = 0
// };
// enum ReferenceType {};

// enum LanguageType {FR = 0, EN};

// const std::map<std::string, LanguageType> languageStr_to_LanguageType = {{"fr", LanguageType(FR)}, {"en", LanguageType(EN)}};
// const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};

BETTER_ENUM(RoleType, char,
    ROOT        = 0, 
    
    INITIATOR   = 1,
    INTERACTING = 2,
    RECIPIENT   = 3,

    CAUSE       = 4,
    TIME        = 5,
    LOCATION    = 6,
    INTENTION   = 7,
    MANNER      = 8
);

}

BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::PathType);
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::JunctionType);
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::LanguageType);
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::RoleType);