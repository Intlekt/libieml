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
    std::ostringstream os_w;                                        \
    for (auto& warn : parser.getSyntaxWarnings()) {                 \
      os_w << warn->to_string() << std::endl;                       \
    }                                                               \
                                                                    \
    EXPECT_EQ(parser.getSyntaxErrors().size(), 0) << os.str();      \
    EXPECT_EQ(parser.getSyntaxWarnings().size(), 0) << os_w.str();  \
}

#define DEBUG_TEST false

#define TEST_PARSE_ERRORS(Str) {                                    \
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
    std::ostringstream os_w;                                        \
    for (auto& warn : parser.getSyntaxWarnings()) {                 \
      os_w << warn->to_string() << std::endl;                       \
    }                                                               \
                                                                    \
    if (DEBUG_TEST) std::cout << os.str() << std::endl;             \
                                                                    \
    EXPECT_NE(parser.getSyntaxErrors().size(), 0);                  \
}


#define TEST_PARSE_WARNINGS(Str) {                                  \
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
    EXPECT_NE(parser.getSyntaxWarnings().size(), 0);                \
}


using namespace ieml::parser;


TEST(ieml_grammar_test_case, empty_string)                                           TEST_PARSE_NO_ERRORS(R"()");

TEST(ieml_grammar_test_case, categry_word_decl)                                      TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".)");
TEST(ieml_grammar_test_case, language_string_with_special_characters)                TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@inflection en:a'-0123456789azAZÀÂÄÇÉÈÊËÎÏÔÖÙÛÜŸàâäçéèêëîïôöùûüÿÆŒæœ class:NOUN "a.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun)                             TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@inflection en:test class:NOUN "a.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_noun)                   TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".@rootparadigm type:inflection "O:M:.".@inflection en:test class:NOUN "a.".  @node fr:test2 (0 ~test #"we.").)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_noun_not_root)          TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".@rootparadigm type:inflection "O:M:.".@inflection en:test class:NOUN "a.".  @node en:test2 (0 ~test #"we."). @node fr:test3 (0 #test2, 1 ~test #test2).)");
TEST(ieml_grammar_test_case, inflection_decl_valid_noun_test_verb)                   TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".@rootparadigm type:inflection "O:M:.".@inflection en:test class:VERB "a.".  @node fr:test2 (0 ~test #"we.").)");
TEST(ieml_grammar_test_case, inflection_decl_valid_verb)                             TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@inflection en:test class:VERB "a.".)");
TEST(ieml_grammar_test_case, inflection_decl_valid_multi_trad)                       TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@inflection en:test en:test2 class:VERB "a.".)");
TEST(ieml_grammar_test_case, component_decl)                                         TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.". @component en:test (0 #"wa."). @component en:test2 (0 #test).)");
TEST(ieml_grammar_test_case, auxiliary_role_integer)                                 TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:8 "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_root)                         TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:ROOT "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_initiator)                    TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:INITIATOR "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_interacting)                  TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:INTERACTING "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_recipient)                    TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:RECIPIENT "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_manner)                       TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:MANNER "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_cause)                        TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:M:.". @auxiliary en:test role:CAUSE "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_time)                         TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:auxiliary "O:M:.".@auxiliary en:test role:TIME "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_location)                     TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:auxiliary "O:M:.".@auxiliary en:test role:LOCATION "a.".)");
TEST(ieml_grammar_test_case, auxiliary_role_identifier_intention)                    TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:auxiliary "O:M:.".@auxiliary en:test role:INTENTION "a.".)");
TEST(ieml_grammar_test_case, paradigm_category)                                      TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.". @node en:ss0 (0 #"wa.", 1 #"wa."). @node en:ss1 (0 #"wa.", 1 #"we."). @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {#"wa."; #"we."}).)");
TEST(ieml_grammar_test_case, paradigm_auxiliary)                                     TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:auxiliary "O:M:.". @rootparadigm type:category "O:O:.". @auxiliary en:aux0 role:1 "e.". @auxiliary en:aux1 role:1 "o.". @node en:ss0 (0 #"wa.", 1 *aux0 #"wa."). @node en:ss1 (0 #"wa.", 1 *aux1 #"wa."). @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {*aux0 ; *aux1} #"wa.").)");
TEST(ieml_grammar_test_case, paradigm_inflection)                                    TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:INFLECTION "O:M:.". @rootparadigm type:category "O:O:.". @inflection en:infl0 class:NOUN "e.". @inflection en:infl1 class:NOUN "o.". @node en:ss0 (0 #"wa.", 1 ~"e." #"wa.").@node en:ss1 (0 #"wa.", 1 ~"o." #"wa."). @node en:invariant (0 #"wa."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {~infl0; ~infl1} #"wa.").)");
TEST(ieml_grammar_test_case, inflection_list_word)                                   TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:INFLECTION "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:infl class:NOUN "e.". @node en:test (0 #"wa.", 1 ~"e." #"wa.").)");
TEST(ieml_grammar_test_case, auxiliary_word)                                         TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:CATEGORY "U:O:.". @rootparadigm type:AUXILIARY "A:O:.". @auxiliary en:aux role:1 "we.". @node en:test (0 #"wa.", 1 *"we." #"wa.").)");
TEST(ieml_grammar_test_case, junction_word)                                          TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:JUNCTION "O:M:.". @rootparadigm type:category "O:O:.".  @junction en:junc "e.". @node en:test (0 #"wa.", 1 &"e." [#"wa." #"wo."]).)");
TEST(ieml_grammar_test_case, paranode_1d)                                            TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:eqweq (0 #"wa."). @node en:ss0 (0 #"wa.", 1 #"wa.").@node en:ss1 (0 #"wa.", 1 #"wo."). @paranode en:fr 1d:/#/1 (0 #"wa.", 1 {#"wa."; #"wo."}).)");
TEST(ieml_grammar_test_case, multiple_paranode_same_invariant)                       TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:test (0 #"wa."). @node en:inv01 (0 #"wa.", 1 #"wa."). @node en:ss32 (0 #"wa.", 2 #"we."). @node en:inv012 (0 #"wa.", 2 #"wa."). @node en:ss3 (0 #"wa.", 1 #"we."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @paranode en:test3 1d:/#/2 (0 #"wa.", 2 {#"wa.";#"we."}).)");

TEST(ieml_grammar_test_case, table_declaration)                                      TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:test (0 #"wa."). @node en:inv01 (0 #"wa.", 1 #"wa."). @node en:ss32 (0 #"wa.", 2 #"we."). @node en:inv012 (0 #"wa.", 2 #"wa."). @node en:ss3 (0 #"wa.", 1 #"we."). @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @paranode en:test3 1d:/#/2 (0 #"wa.", 2 {#"wa.";#"we."}). @table #test -> test2.)");
TEST(ieml_grammar_test_case, table_declaration_twice)                                TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:test (0 #"wa."). @node en:inv01 (0 #"wa.", 1 #"wa."). @node en:ss32 (0 #"wa.", 2 #"we."). @node en:inv012 (0 #"wa.", 2 #"wa."). @node en:ss3 (0 #"wa.", 1 #"we.").  @paranode en:test2 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @paranode en:test3 1d:/#/2 (0 #"wa.", 2 {#"wa.";#"we."}). @table #test -> test2.@table #test -> test3.)");
TEST(ieml_grammar_test_case, table_declaration_2_level)                              TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @node en:inv01 (0 #"wa.", 1 #"wa."). @node en:ss3 (0 #"wa.", 1 #"we."). @node en:ss0 (0 #"wa.", 1 #"wa.", 2 #"wa."). @node en:ss1 (0 #"wa.", 1 #"wa.", 2 #"we.").@paranode en:para01 1d:/#/2 (0 #"wa.", 1 #"wa.", 2 {#"wa.";#"we."}).  @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}).  @table #inv0 -> para0 #inv01 -> para01.)");

TEST(ieml_grammar_test_case, root_paradigm)                                          TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:CATEGORY "O:".@rootparadigm type:INFLECTION "M:M:.".@rootparadigm type:AUXILIARY "O:O:.".@rootparadigm type:JUNCTION "M:M:.e.-".)");
TEST(ieml_grammar_test_case, root_paradigm_category_define)                          TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:CATEGORY "O:". @node fr:test (0 #"A:").)");
TEST(ieml_grammar_test_case, root_paradigm_inflection_define)                        TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:INFLECTION "O:". @inflection fr:test class:NOUN "A:".)");
TEST(ieml_grammar_test_case, root_paradigm_auxiliary_define)                         TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:AUXILIARY "O:". @auxiliary fr:test role:LOCATION "A:".)");
TEST(ieml_grammar_test_case, root_paradigm_junction_define)                          TEST_PARSE_NO_ERRORS(R"(@rootparadigm type:JUNCTION "O:". @junction fr:test "A:".)");


// ERRORS
TEST(ieml_grammar_test_case, no_root)                                                TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:noun class:NOUN "a.". @component en:test (1 ~noun #"wa.") .)");
TEST(ieml_grammar_test_case, extra_comma_in_phrase_line)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:noun class:VERB "a.". @component en:test (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_with_old_decl)                          TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:noun class:VERB "a.". @component fr'test' (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, language_string_starts_with_digits)                     TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:noun class:VERB "a.". @component en:0 (0 #(0 ~noun #"wa."),) .)");
TEST(ieml_grammar_test_case, invalid_translation_id)                                 TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:M:.".@rootparadigm type:category "O:O:.". @inflection en:noun class:VERB "a.". @component rr:test (0 ~noun #"wa.").)");
TEST(ieml_grammar_test_case, repeated_identifier)                                    TEST_PARSE_ERRORS(R"(@word "a.". @word "b.". @node en:node1 (0 #"a.", 1 #"a."). @node en:node1 (0 #"a.", 1 #"b.").)");
TEST(ieml_grammar_test_case, invalid_identifier)                                     TEST_PARSE_ERRORS(R"(@component en:test (0 #a).)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_type)                        TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:O:.".@inflection en:test "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_type)                           TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:O:.".@inflection en:test invalidtype "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_trad)                        TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:O:.".@inflection class:NOUN "wa.".)");
TEST(ieml_grammar_test_case, inflection_decl_invalid_no_word)                        TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:O:.". @inflection en:test NOUN .)");
TEST(ieml_grammar_test_case, paradigm_category_invalid)                              TEST_PARSE_ERRORS(R"(@word "a.". @word "b.". @paranode fr:test2 1d:/#/0 (0 #{"a."; #"b."}).)");
                
TEST(ieml_grammar_test_case, invalid_paradigm)                                       TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:test2 (0 {#"wa."; #"we."}).)");
TEST(ieml_grammar_test_case, invalid_non_paradigm)                                   TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @paranode en:test2 1d:/#/0 (0 #"wa.").)");
                
TEST(ieml_grammar_test_case, invalid_auxiliary_number)                               TEST_PARSE_ERRORS(R"(@rootparadigm type:auxiliary "O:O:.".@auxiliary en:test role:2 "wa.". @word "e.". @node en:test2 (0 *test #"e.").)");
TEST(ieml_grammar_test_case, invalid_inflection)                                     TEST_PARSE_ERRORS(R"(@rootparadigm type:inflection "O:O:.".@inflection en:test class:VERB "wa.". @word "e.". @node en:test2 (0 ~test #"e."). @node en:test3 (0 #test2, 1 ~test #test2). )");
TEST(ieml_grammar_test_case, paradigm_empty_invariant)                               TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @paranode en:test2 1d:/#/0 (0 {#"wa."; #"we."}).)");
TEST(ieml_grammar_test_case, paradigm_invalid_invariant)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:invariant (1 #"wa."). @paranode en:test2 1d:/#/0 (0 {#"wa."; #"we."}, 1 #"wa.").)");

TEST(ieml_grammar_test_case, paradigm_invalid_variation)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".@node en:eqweq (0 #"wa.", 1 #"wa.").@paranode en:fr 1d:/#/1 (0 #"wa.", 1 {#"wa."; #"wa."}).)");
TEST(ieml_grammar_test_case, paradigm_invalid_variation_2)                           TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:eqweq (0 #"wa."). @paranode en:fr 1d:/#/0/"wa." (0 #"wa.", 1 {#"wa."; #"wo."}).)");
TEST(ieml_grammar_test_case, paradigm_invalid_dimension)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".@node en:eqweq (0 #"wa.").@paranode en:fr 2d:/#/1 (0 #"wa.", 1 {#"wa."; #"wo."}).)");
TEST(ieml_grammar_test_case, paradigm_invalid_dimension_not_a_variation)             TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:eqweq (0 #"wa."). @paranode en:fr 1d:/#/0 (0 #"wa.", 1 {#"wa."; #"wo."}).)");
TEST(ieml_grammar_test_case, table_declaration_2_level_invalid_order)                TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @node en:inv01 (0 #"wa.", 1 #"wa."). @paranode en:para01 1d:/#/2 (0 #"wa.", 1 #"wa.", 2 {#"wa.";#"we."}). @table #inv01 -> para01 #inv0 -> para0 .)");
TEST(ieml_grammar_test_case, table_declaration_invalid_invariant)                    TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @node en:test (0 #"wa.", 1 #"wa."). @table #test -> para0 .)");
TEST(ieml_grammar_test_case, table_declaration_no_paranode)                          TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @node en:test (0 #"wa.", 1 #"wa."). @table #test -> inv0 .)");
TEST(ieml_grammar_test_case, table_declaration_no_invariant)                         TEST_PARSE_ERRORS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}). @node en:test (0 #"wa.", 1 #"wa."). @paranode en:para01 1d:/#/2 (0 #"wa.", 1 #"wa.", 2 {#"wa.";#"we."}). @table #para0 -> para01 .)");

TEST(ieml_grammar_test_case, root_paradigm_invalid_type)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:TEST "O:".)");
TEST(ieml_grammar_test_case, root_paradigm_not_a_paradigm)                           TEST_PARSE_ERRORS(R"(@rootparadigm type:CATEGORY "a.".)");
TEST(ieml_grammar_test_case, root_paradigm_overlap)                                  TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "S:+B:M:.".@rootparadigm type:CATEGORY "M:M:.".)");
TEST(ieml_grammar_test_case, root_paradigm_multiple_same_word)                       TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "O:".@rootparadigm type:CATEGORY "O:".)");
TEST(ieml_grammar_test_case, root_paradigm_invalid_script)                           TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "test".)");
TEST(ieml_grammar_test_case, root_paradigm_empty_script)                             TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "".)");
TEST(ieml_grammar_test_case, root_paradigm_category_not_defined)                     TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "O:". @node fr:test (0 #"A:").)");
TEST(ieml_grammar_test_case, root_paradigm_inflection_not_defined)                   TEST_PARSE_ERRORS(R"(@rootparadigm type:AUXILIARY "O:". @inflection fr:test class:NOUN "A:".)");
TEST(ieml_grammar_test_case, root_paradigm_auxiliary_not_defined)                    TEST_PARSE_ERRORS(R"(@rootparadigm type:INFLECTION "O:". @auxiliary fr:test role:LOCATION "A:".)");
TEST(ieml_grammar_test_case, root_paradigm_junction_not_defined)                     TEST_PARSE_ERRORS(R"(@rootparadigm type:AUXILIARY "O:". @junction fr:test "A:".)");


// WARNINGS
TEST(ieml_grammar_test_case, test_warning_missing_singular_sequence)                 TEST_PARSE_WARNINGS(R"(@rootparadigm type:category "O:O:.".  @node en:inv0 (0 #"wa."). @paranode en:para0 1d:/#/1 (0 #"wa.", 1 {#"wa.";#"we."}).)");
