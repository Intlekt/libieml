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

TEST(ieml_grammar_test_case, paradigm_definition)
{
    PARSE_NO_ERRORS(R"(@word "a.". @word "b.". @node en:invariant (0 #"a."). @node en:node0 (0 #"a.", 1 #"a."). @node en:node1 (0 #"a.", 1 #"b."). @paranode en:paranode 1d:/#/1 (0 #"a.", 1 {#"a.";#"b."}).)");

    const auto &ctx = parser.getContext();
    const auto &creg = ctx->getCategoryRegister();

    const auto &paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);
    EXPECT_EQ(creg.getDefinitionType(paradigm), +ieml::structure::DefinitionType::PARADIGM);
    ASSERT_TRUE(paradigm->is_paradigm());

    const auto &node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto &node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));

    EXPECT_EQ(ieml::structure::PathTree::singular_sequences(paradigm), (ieml::structure::PathTree::Vector{node0, node1}));

    EXPECT_EQ(paradigm->getChildrenAsVector()[0]->getChildrenAsVector()[0], node0);
    EXPECT_EQ(paradigm->getChildrenAsVector()[1]->getChildrenAsVector()[0], node1);
};

TEST(ieml_grammar_test_case, paradigm_definition_2d)
{
    PARSE_NO_ERRORS(R"(@word "a.". @word "b.".
                       @node en:invariant (0 #"a.").
                       @node en:node0 (0 #"a.", 1 #"a.", 2 #"a.").
                       @node en:node1 (0 #"a.", 1 #"b.", 2 #"a.").
                       @node en:node2 (0 #"a.", 1 #"a.", 2 #"b.").
                       @node en:node3 (0 #"a.", 1 #"b.", 2 #"b.").
                       @paranode en:paranode 1d:/#/1 2d:/#/2 (0 #"a.", 1 {#"a.";#"b."}, 2 {#"a.";#"b."}).)");

    const auto &ctx = parser.getContext();
    const auto &creg = ctx->getCategoryRegister();
    const auto &preg = ctx->getParadigmRegister();

    const auto &paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    const auto &node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto &node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));
    const auto &node2 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node2"));
    const auto &node3 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node3"));

    ASSERT_NE(paradigm, nullptr);
    ASSERT_NE(node0, nullptr);
    ASSERT_NE(node1, nullptr);
    ASSERT_NE(node2, nullptr);
    ASSERT_NE(node3, nullptr);

    ASSERT_EQ(ieml::structure::PathTree::singular_sequences(paradigm),
              ieml::structure::PathTree::Vector({node0, node1, node2, node3}));
};

TEST(ieml_grammar_test_case, paradigm_register)
{
    PARSE_NO_ERRORS(R"(@word "a.". @word "b.". @node en:invariant (0 #"a."). @paranode en:paranode 1d:/#/1 (0 #"a.", 1 {#"a.";#"b."}).)");
    const auto &ctx = parser.getContext();
    const auto &creg = ctx->getCategoryRegister();
    const auto &preg = ctx->getParadigmRegister();

    const auto &invariant = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "invariant"));
    const auto &paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);
    ASSERT_NE(invariant, nullptr);

    const auto &paradigms_resolved = preg.resolve_paradigms(invariant);

    ASSERT_EQ(paradigms_resolved.size(), 1);
    ASSERT_EQ(*paradigms_resolved.begin(), paradigm);
}

TEST(ieml_grammar_test_case, paradigm_dimension_definition)
{
    PARSE_NO_ERRORS(R"(@word "a.". @word "b.".
                       @node en:invariant (0 #"a.").
                       @node en:node0 (0 #"a.", 1 #"a.", 2 #"a.").
                       @node en:node1 (0 #"a.", 1 #"a.", 2 #"b.").
                       @node en:node2 (0 #"a.", 1 #"b.", 2 #"a.").
                       @node en:node3 (0 #"a.", 1 #"b.", 2 #"b.").
                       @paranode en:paranode 1d:/#/1 2d:/#/2 (0 #"a.", 1 {#"b.";#"a."}, 2 {#"b.";#"a."}).)");

    const auto &ctx = parser.getContext();
    const auto &creg = ctx->getCategoryRegister();
    const auto &preg = ctx->getParadigmRegister();

    const auto &paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);

    const auto &table = preg.get_table(paradigm);

    ASSERT_EQ(table->getShapeTableNd().size(), 3);
    ASSERT_EQ(table->getShapeTableNd()[0], 2);
    ASSERT_EQ(table->getShapeTableNd()[1], 2);
    ASSERT_EQ(table->getShapeTableNd()[2], 1);

    const auto &node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto &node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));
    const auto &node2 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node2"));
    const auto &node3 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node3"));

    ASSERT_EQ(table->at(0, {0, 0, 0}), node0);
    ASSERT_EQ(table->at(0, {0, 1, 0}), node1);
    ASSERT_EQ(table->at(0, {1, 0, 0}), node2);
    ASSERT_EQ(table->at(0, {1, 1, 0}), node3);
}

TEST(ieml_grammar_test_case, no_regression_missing_invariant)
{
    {
        PARSE_NO_ERRORS(R"(
            @word "a.". @word "b.". @word "c.".
            @node en:node0 (0 #"a.", 1 #"a.", 2 #"a.").
            @paranode en:paranode00 1d:/#/3 (0 #"a.", 1 #"a.", 2 #"a.", 3 {#"a.";#"b.";#"c."}).)");
    }
    {
        PARSE_NO_ERRORS(R"(@word "a.". @word "b.".
                       @node en:node0 (0 #"a.").
                       @node en:node1 (0 #"a.", 1 #"a.").
                       @node en:invariant (0 #node0, 1 #node1).
                       @paranode en:paranode 1d:/#/4 (0 #node0, 1 #node1, 4 {#"b.";#"a."}).)");
    }
    {
        // the variation has some path in common
        PARSE_NO_ERRORS(R"(@word "a.". @word "b.". @word "c.". @word "d.".
                       @node en:var1 (0 #"d.", 1 #"b.").
                       @node en:var2 (0 #"c.", 1 #"b.").
                       @node en:invariant (0 #"a.", 1 #"b.").
                       @paranode en:paradigm 1d: /#/2 (0 #"a.", 1 #"b.", 2 {#var1; #var2}).)");
    }
    // This test ensure that subphrase are supported in paranode. (not supported)
    // {
    //     // the variation is in a sub phrase
    //     PARSE_NO_ERRORS(R"(@word "a.". @word "b.". @word "c.". @word "d.".
    //                    @node en:var1 (0 #"d.", 1 #"b.").
    //                    @node en:var2 (0 #"c.", 1 #"b.").
    //                    @node en:invariant (0 #"a.", 1 #"b.", 2 #(0 #"a.")).
    //                    @paranode en:paradigm 1d: /#/2 (0 #"a.", 1 #"b.", 2 #(0 #"a.", 1 {#var1; #var2})).)");
    // }
    {
        // the variation is in a sub phrase.
        PARSE_NO_ERRORS(R"(@word "a.". @word "b.". @word "c.". @word "d.".
                       @node en:var1 (0 #"d.", 1 #"b.").
                       @node en:var2 (0 #"c.", 1 #"b.").
                       @node en:invariant (0 #"a.", 1 #"b.").
                       @paranode en:paradigm 1d: /#/2 (0 #"a.", 1 #"b.", 2 #(0 #"a.", 1 {#var1; #var2})).)");
    }
}

TEST(ieml_grammar_test_case, no_regression_crash_order_of_definition)
{
    PARSE_NO_ERRORS(R"(
        @word "A:A:A:.".
        @word "B:B:B:.".
        @word "T:T:T:.".

        @node en:invariant (0 #"A:A:A:.").
        @paranode en:paranode 1d:/#/1 (0 #"A:A:A:.", 1 { #(0 #"B:B:B:.") ; #(0 #"T:T:T:.")}).
        @node en:should not crash (0 #(0 #"B:B:B:.")).
    )");
}