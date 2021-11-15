#pragma once

#include <string>
#include <vector>
#include <map>


namespace ieml::AST {


enum JunctionType {};
enum ReferenceType {};

enum LanguageType {FR = 0, EN};

const std::map<std::string, LanguageType> languageStr_to_LanguageType = {{"fr", LanguageType(FR)}, {"en", LanguageType(EN)}};
const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};

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
enum DeclarationType { Component, Node };

}