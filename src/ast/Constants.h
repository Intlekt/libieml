#pragma once

#include <string>

namespace ieml {
namespace AST {


enum JunctionType {};
enum ReferenceType {};

enum LanguageType {FR = 0, EN};
const std::string languages_names[LanguageType::EN + 1] = {"fr", "en"};

enum RoleType {};
enum DeclarationType { Component, Node };

typedef std::vector<std::string> TranslationList;
typedef std::map<LanguageType, TranslationList> TranslationsMap;

}
}