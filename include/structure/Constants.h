#pragma once

#include <enum.h>


namespace ieml::structure {

BETTER_ENUM(DefinitionType, char, NODE, COMPONENT, PARADIGM)

BETTER_ENUM(ElementType, char, WORD, PATH_TREE)

BETTER_ENUM(PathType, char,
    PARADIGM,
    PARADIGM_INDEX,
    ROOT,
    ROLE,
    JUNCTION_PHRASE,
    JUNCTION_PHRASE_INDEX,
    JUNCTION_AUXILIARY,
    JUNCTION_AUXILIARY_INDEX,
    AUXILIARY,
    JUNCTION_INFLECTION,
    JUNCTION_INFLECTION_INDEX,
    INFLECTION,
    JUNCTION_CATEGORY,
    JUNCTION_CATEGORY_INDEX,
    WORD
)

BETTER_ENUM(InflectionType, char, VERB, NOUN)

BETTER_ENUM(LanguageType, char, FR = 1, EN)

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
)

BETTER_ENUM(WordType, char, CATEGORY, INFLECTION, AUXILIARY, JUNCTION)

}

BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::PathType)
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::LanguageType)
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::RoleType)
BETTER_ENUMS_DECLARE_STD_HASH(ieml::structure::WordType)
