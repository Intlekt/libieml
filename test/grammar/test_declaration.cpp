/**
 * @file test_declaration.cpp
 * @author Louis van Beurden (louis.vanbeurden@gmail.com)
 * @brief Test small expressions to validate the parser rules. If the parser segfault, add the test expression here.
 * @version 0.1
 * @date 2022-01-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */


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



TEST(ieml_grammar_test_case, categry_word_decl)                                      TEST_PARSE_NO_ERRORS(R"(@word "wa.".)");
TEST(ieml_grammar_test_case, language_string_with_special_characters)                TEST_PARSE_NO_ERRORS(R"(@inflection en:a'-0123456789azAZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun)                             TEST_PARSE_NO_ERRORS(R"(@inflection en:test NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_noun)                   TEST_PARSE_NO_ERRORS(R"(@inflection en:test NOUN "wa.". @word "we.". @node fr:test2 (0 ~test #"we.").)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_noun_not_root)          TEST_PARSE_NO_ERRORS(R"(@inflection en:test NOUN "wa.". @word "we.". @node en:test2 (0 ~test #"we."). @node fr:test3 (0 #test2, 1 ~test #test2).)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_verb)                   TEST_PARSE_NO_ERRORS(R"(@inflection en:test VERB "wa.". @word "we.". @node fr:test2 (0 ~test #"we.").)");
TEST(ieml_grammar_test_case, inflection_decl_valid_verb)                             TEST_PARSE_NO_ERRORS(R"(@inflection en:test VERB "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_multi_trad)                       TEST_PARSE_NO_ERRORS(R"(@inflection en:test en:test2 VERB "wa.".)");
TEST(ieml_grammar_test_case, component_decl)                                         TEST_PARSE_NO_ERRORS(R"(@word "wa.". @component en:test (0 #"wa."). @component en:test2 (0 #test).)");
TEST(ieml_grammar_test_case, auxiliary_role_integer)                                 TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test 8 "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_root)                         TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test ROOT "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_initiator)                    TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test INITIATOR "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_interacting)                  TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test INTERACTING "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_recipient)                    TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test RECIPIENT "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_manner)                       TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test MANNER "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_cause)                        TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test CAUSE "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_time)                         TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test TIME "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_location)                     TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test LOCATION "a".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_intention)                    TEST_PARSE_NO_ERRORS(R"(@auxiliary en:test INTENTION "a".)");
TEST(ieml_grammar_test_case, paradigm_category)                                      TEST_PARSE_NO_ERRORS(R"(@word "wa.". @word "we.". @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 #{"wa."; "we."}).)");
TEST(ieml_grammar_test_case, paradigm_auxiliary)                                     TEST_PARSE_NO_ERRORS(R"(@word "wa.". @auxiliary en:aux0 1 "we.". @auxiliary en:aux1 1 "wo.". @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {*aux0 ; *aux1} #"wa.").)");
TEST(ieml_grammar_test_case, paradigm_inflection)                                    TEST_PARSE_NO_ERRORS(R"(@word "wa.". @inflection en:infl0 NOUN "we.". @inflection en:infl1 NOUN "wo.". @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {~infl0; ~infl1} #"wa.").)");
TEST(ieml_grammar_test_case, inflection_list_word)                                   TEST_PARSE_NO_ERRORS(R"(@word "wa.". @inflection en:infl NOUN "we.". @node en:test (0 #"wa.", 1 ~"we." #"wa.").)");
TEST(ieml_grammar_test_case, auxiliary_word)                                         TEST_PARSE_NO_ERRORS(R"(@word "wa.". @auxiliary en:aux 1 "we.". @node en:test (0 #"wa.", 1 *"we." #"wa.").)");
TEST(ieml_grammar_test_case, junction_word)                                          TEST_PARSE_NO_ERRORS(R"(@word "wa.". @word "wo.". @junction en:junc "we.". @node en:test (0 #"wa.", 1 &"we." [#"wa." #"wo."]).)");


                
TEST(ieml_grammar_test_case, no_root)                                                TEST_PARSE_ERRORS(R"(@word "wa.". @inflection en:noun NOUN "a.". @component en:test (1 ~noun #"wa.") .)");
TEST(ieml_grammar_test_case, extra_comma_in_phrase_line)                             TEST_PARSE_ERRORS(R"(@word "wa.". @inflection en:noun VERB "a.". @component en:test (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_with_old_decl)                          TEST_PARSE_ERRORS(R"(@word "wa.". @inflection en:noun VERB "a.". @component fr'test' (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_starts_with_digits)                     TEST_PARSE_ERRORS(R"(@word "wa.". @inflection en:noun VERB "a.". @component en:0 (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, invalid_translation_id)                                 TEST_PARSE_ERRORS(R"(@word "wa.". @inflection en:noun VERB "a.". @component rr:test (0 ~noun #"wa.").)");
TEST(ieml_grammar_test_case, repeated_identifier)                                    TEST_PARSE_ERRORS(R"(@word "a". @word "b". @node en:node1 (0 #"a", 1 #"a"). @node en:node1 (0 #"a", 1 #"b").)");
TEST(ieml_grammar_test_case, invalid_identifier)                                     TEST_PARSE_ERRORS(R"(@component en:test (0 #a).)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_type)                        TEST_PARSE_ERRORS(R"(@inflection en:test "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_type)                           TEST_PARSE_ERRORS(R"(@inflection en:test invalidtype "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_trad)                        TEST_PARSE_ERRORS(R"(@inflection NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_word)                        TEST_PARSE_ERRORS(R"(@inflection en:test NOUN .)");
TEST(ieml_grammar_test_case, paradigm_category_invalid)                              TEST_PARSE_ERRORS(R"(@word "a". @word "b". @paranode fr:test2 (0 #{"a"; #"b"}).)");
                
TEST(ieml_grammar_test_case, invalid_paradigm)                                       TEST_PARSE_ERRORS(R"(@word "wa.". @word "we.". @node en:test2 (0 #{"wa."; "we."}).)");
TEST(ieml_grammar_test_case, invalid_non_paradigm)                                   TEST_PARSE_ERRORS(R"(@word "wa.". @word "we.". @paranode en:test2 (0 #"wa.").)");
                
TEST(ieml_grammar_test_case, invalid_auxiliary_number)                               TEST_PARSE_ERRORS(R"(@auxiliary en:test 2 "wa.". @word "we.". @node en:test2 (0 *test #"we.").)");
TEST(ieml_grammar_test_case, invalid_inflection)                                     TEST_PARSE_ERRORS(R"(@inflection en:test VERB "wa.". @word "we.". @node en:test2 (0 ~test #"we."). @node en:test3 (0 #test2, 1 ~test #test2). )");
TEST(ieml_grammar_test_case, multiple_paranode_same_invariant)                       TEST_PARSE_ERRORS(R"(@word "wa.". @word "we.". @node en:test (0 #"wa."). @paranode en:test2 (0 #"wa.", 1 #{"wa.";"we."}). @paranode en:test3 (0 #"wa.", 2 #{"wa.";"we."}).)");
TEST(ieml_grammar_test_case, paradigm_empty_invariant)                               TEST_PARSE_ERRORS(R"(@word "wa.". @word "we.". @paranode en:test2 (0 #{"wa."; "we."}).)");
TEST(ieml_grammar_test_case, paradigm_invalid_invariant)                             TEST_PARSE_ERRORS(R"(@word "wa.". @word "we.". @node en:invariant (1 #"wa."). @paranode en:test2 (0 #{"wa."; "we."}, 1 #"wa.").)");
                