#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"
#include "utils_testing.h"

#include "IemlParser.h"


using namespace ieml::parser;


TEST(ieml_grammar_test_case, phrase_word) {
    PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.". @node en:test (0 #"we."). @node en:test2 (0 #"we.", 1 #"wa.").)");
    auto& ctx = *parser.getContext();
    auto& creg = ctx.getCategoryRegister();
    auto& wreg = ctx.getWordRegister();

    const auto& word = wreg.resolve_category_word(ctx.get_or_parse_script("we."));
    const auto& node = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "test"));

    const auto pw_it_pair = creg.get_phrase_words(word);
    for (auto it = pw_it_pair.first; it != pw_it_pair.second; it ++)
        ASSERT_EQ(node, it->second);
}