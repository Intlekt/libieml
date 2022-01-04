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

TEST(ieml_grammar_test_case, categry_word_decl)                     TEST_PARSE_NO_ERRORS(R"(@word "wa.".)");

TEST(ieml_grammar_test_case, language_string_with_special_characters)             TEST_PARSE_NO_ERRORS(R"(@inflection fr:a'-0123456789azAZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun)             TEST_PARSE_NO_ERRORS(R"(@inflection fr:test NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_verb)             TEST_PARSE_NO_ERRORS(R"(@inflection fr:test VERB "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_multi_trad)       TEST_PARSE_NO_ERRORS(R"(@inflection fr:test fr:test2 VERB "wa.".)");
TEST(ieml_grammar_test_case, component_decl)                         TEST_PARSE_NO_ERRORS(R"(@word "wa.". @component fr:test (0 #"wa."). @component fr:test2 (0 #test).)");
TEST(ieml_grammar_test_case, paradigm)                         TEST_PARSE_NO_ERRORS(R"(@word "wa.". @word "we.". @component fr:test2 (0 #{"wa."; "we."}).)");

TEST(ieml_grammar_test_case, extra_comma_in_phrase_line)             TEST_PARSE_ERRORS(R"(@component fr:test (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_with_old_decl)             TEST_PARSE_ERRORS(R"(@component fr'test' (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_starts_with_digits)             TEST_PARSE_ERRORS(R"(@component fr:0 (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, invalid_translation_id)                 TEST_PARSE_ERRORS(R"(@component rr:test (0 ~noun #"wa.").)");
TEST(ieml_grammar_test_case, invalid_identifier)                     TEST_PARSE_ERRORS(R"(@component fr:test (0 ~noun #a).)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_type)        TEST_PARSE_ERRORS(R"(@inflection fr:test "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_type)           TEST_PARSE_ERRORS(R"(@inflection fr:test invalidtype "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_trad)        TEST_PARSE_ERRORS(R"(@inflection NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_word)        TEST_PARSE_ERRORS(R"(@inflection fr:test NOUN .)");

TEST(ieml_grammar_test_case, invalid_auxiliary_number)        TEST_PARSE_ERRORS(R"(@auxiliary fr:test 2 "wa.". @word "we.". @node fr:test2 (0 *test #"we.").)");
TEST(ieml_grammar_test_case, invalid_inflection)              TEST_PARSE_ERRORS(R"(@inflection fr:test NOUN "wa.". @word "we.". @node fr:test2 (0 ~test #"we.").)");