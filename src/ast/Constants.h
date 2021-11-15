#pragma once

#include <string>
#include <vector>
#include <map>

#include "ast/Identifier.h"

namespace ieml {
namespace AST {


enum JunctionType {};
enum ReferenceType {};

enum LanguageType {FR = 0, EN};

const std::map<std::string, LanguageType> languageStr_to_LanguageType = {{"fr", LanguageType(FR)}, {"en", LanguageType(EN)}};
const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};

enum RoleType {};
enum DeclarationType { Component, Node };

typedef std::vector<Identifier> TranslationList;
typedef std::map<LanguageType, TranslationList> TranslationsMap;

}
}