#pragma once

#include <enum.h>


namespace ieml::AST {

BETTER_ENUM(DeclarationType, char, COMPONENT, NODE, PARANODE, LINK, WORD, INFLECTION, AUXILIARY, JUNCTION, LANGUAGE, TABLE, FUNCTION)
BETTER_ENUM(PathType, char, ROOT, JUNCTION, PARADIGM, ROLE, WORD, AUXILIARY, INFLECTION)
BETTER_ENUM(FunctionType, char, WORD, PHRASE)
BETTER_ENUM(BooleanWordFunctionConditionExprOperatorType, char, OR, AND)

BETTER_ENUM(WordAccessorType, char, SUBSTANCE, ATTRIBUTE, MODE)
}