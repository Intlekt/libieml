#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"


#define TEST_PARSE_NO_ERRORS(Str) {                                 \
    IEMLParser parser(Str);                                         \
    try {                                                           \
      parser.parse();                                               \
    } catch (std::exception& e) {                                   \
      EXPECT_TRUE(false) << e.what();                               \
    }                                                               \
                                                                    \
    std::ostringstream os;                                          \
    for (auto& error : parser.getSyntaxErrors()) {                  \
      os << error->to_string() << std::endl;                        \
    }                                                               \
                                                                    \
    EXPECT_EQ(parser.getSyntaxErrors().size(), 0) << os.str();      \
}

#define TEST_PARSE_ERRORS(Str) {                                    \
    IEMLParser parser(Str);                                         \
    try {                                                           \
      parser.parse();                                               \
    } catch (std::exception& e) {                                   \
      EXPECT_TRUE(false) << e.what();                               \
    }                                                               \
                                                                    \
    EXPECT_NE(parser.getSyntaxErrors().size(), 0);                  \
}

using namespace ieml::parser;

TEST(ieml_grammar_test_case, inflexing_decl_valid_noun)             TEST_PARSE_NO_ERRORS(R"(@inflexing fr"test" NOUN 'wa.'.)");
TEST(ieml_grammar_test_case, inflexing_decl_valid_verb)             TEST_PARSE_NO_ERRORS(R"(@inflexing fr"test" VERB 'wa.'.)");
TEST(ieml_grammar_test_case, inflexing_decl_valid_multi_trad)       TEST_PARSE_NO_ERRORS(R"(@inflexing fr"test" fr"test2" VERB 'wa.'.)");
TEST(ieml_grammar_test_case, component_decl)                        TEST_PARSE_NO_ERRORS(R"(@component fr"test" (0 #'wa.'). @component fr"test2" (0 #test).)");

TEST(ieml_grammar_test_case, extra_comma_in_phrase_line)            TEST_PARSE_ERRORS(R"(@component fr"test" (0 #(0 ~noun #'wa.'),) .)");
TEST(ieml_grammar_test_case, invalid_translation_id)                TEST_PARSE_ERRORS(R"(@component rr"test" (0 ~noun #'wa.').)");
TEST(ieml_grammar_test_case, invalid_identifier)                    TEST_PARSE_ERRORS(R"(@component fr"test" (0 ~noun #a).)");
TEST(ieml_grammar_test_case, inflexing_decl_invalid_no_type)        TEST_PARSE_ERRORS(R"(@inflexing fr"test" 'wa.'.)");
TEST(ieml_grammar_test_case, inflexing_decl_invalid_type)           TEST_PARSE_ERRORS(R"(@inflexing fr"test" invalidtype 'wa.'.)");
TEST(ieml_grammar_test_case, inflexing_decl_invalid_no_trad)        TEST_PARSE_ERRORS(R"(@inflexing NOUN 'wa.'.)");
TEST(ieml_grammar_test_case, inflexing_decl_invalid_no_word)        TEST_PARSE_ERRORS(R"(@inflexing fr"test" NOUN .)");