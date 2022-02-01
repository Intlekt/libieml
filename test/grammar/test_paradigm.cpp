#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"
#include "test_utils.h"

#include "IemlParser.h"


using namespace ieml::parser;


TEST(ieml_grammar_test_case, paradigm_definition) {
    PARSE_NO_ERRORS(R"(@word "a". @word "b". @node en:invariant (0 #"a"). @node en:node0 (0 #"a", 1 #"a"). @node en:node1 (0 #"a", 1 #"b"). @paranode en:paranode 1d:/#/1 (0 #"a", 1 #{"a";"b"}).)");

    const auto& ctx = parser.getContext();
    const auto& creg = ctx->getCategoryRegister();

    const auto& paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);
    EXPECT_EQ(creg.getDefinitionType(paradigm), +ieml::structure::DefinitionType::PARADIGM);
    ASSERT_TRUE(paradigm->is_paradigm());

    const auto& node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto& node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));

    EXPECT_EQ(ieml::structure::PathTree::singular_sequences(paradigm), (ieml::structure::PathTree::Vector{node0, node1}));

    EXPECT_EQ(paradigm->getChildrenAsVector()[0]->getChildrenAsVector()[0], node0);
    EXPECT_EQ(paradigm->getChildrenAsVector()[1]->getChildrenAsVector()[0], node1);
};

TEST(ieml_grammar_test_case, paradigm_definition_2d) {
    PARSE_NO_ERRORS(R"(@word "a". @word "b". 
                       @node en:invariant (0 #"a").
                       @node en:node0 (0 #"a", 1 #"a", 2 #"a").
                       @node en:node1 (0 #"a", 1 #"b", 2 #"a").
                       @node en:node2 (0 #"a", 1 #"a", 2 #"b").
                       @node en:node3 (0 #"a", 1 #"b", 2 #"b").
                       @paranode en:paranode 1d:/#/1 2d:/#/2 (0 #"a", 1 #{"a";"b"}, 2 #{"a";"b"}).)");

    const auto& ctx = parser.getContext();
    const auto& creg = ctx->getCategoryRegister();
    const auto& preg = ctx->getParadigmRegister();

    const auto& paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    const auto& node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto& node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));
    const auto& node2 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node2"));
    const auto& node3 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node3"));

    ASSERT_EQ(ieml::structure::PathTree::singular_sequences(paradigm), 
              ieml::structure::PathTree::Vector({node0, node1, node2, node3}));
};


TEST(ieml_grammar_test_case, paradigm_register) {
    PARSE_NO_ERRORS(R"(@word "a". @word "b". @node en:invariant (0 #"a"). @paranode en:paranode 1d:/#/1 (0 #"a", 1 #{"a";"b"}).)");
    const auto& ctx = parser.getContext();
    const auto& creg = ctx->getCategoryRegister();
    const auto& preg = ctx->getParadigmRegister();

    const auto& invariant = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "invariant"));
    const auto& paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);
    ASSERT_NE(invariant, nullptr);

    const auto& paradigm_resolved = preg.resolve_paradigm(invariant);

    ASSERT_EQ(paradigm_resolved, paradigm);
}


TEST(ieml_grammar_test_case, paradigm_dimension_definition) {
    PARSE_NO_ERRORS(R"(@word "a". @word "b". 
                       @node en:invariant (0 #"a").
                       @node en:node0 (0 #"a", 1 #"a", 2 #"a").
                       @node en:node1 (0 #"a", 1 #"a", 2 #"b").
                       @node en:node2 (0 #"a", 1 #"b", 2 #"a").
                       @node en:node3 (0 #"a", 1 #"b", 2 #"b").
                       @paranode en:paranode 1d:/#/1 2d:/#/2 (0 #"a", 1 #{"a";"b"}, 2 #{"a";"b"}).)");

    const auto& ctx = parser.getContext();
    const auto& creg = ctx->getCategoryRegister();
    const auto& preg = ctx->getParadigmRegister();

    const auto& paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    ASSERT_NE(paradigm, nullptr);

    const auto& layout = preg.get_layout(paradigm);

    ASSERT_EQ(layout.getShape().size(), 2);
    ASSERT_EQ(layout.getShape()[0], 2);
    ASSERT_EQ(layout.getShape()[1], 2);

    const auto& node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto& node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));
    const auto& node2 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node2"));
    const auto& node3 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node3"));

    ASSERT_EQ(layout.getCoordinate(node0), ieml::structure::ParadigmLayout::Coordinate({0, 0}));
    ASSERT_EQ(layout.getCoordinate(node1), ieml::structure::ParadigmLayout::Coordinate({0, 1}));
    ASSERT_EQ(layout.getCoordinate(node2), ieml::structure::ParadigmLayout::Coordinate({1, 0}));
    ASSERT_EQ(layout.getCoordinate(node3), ieml::structure::ParadigmLayout::Coordinate({1, 1}));
}
