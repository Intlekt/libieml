#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"


using namespace ieml::parser;


TEST(ieml_grammar_test_case, paradigm_definition) {
    IEMLParser parser(R"(@word "a". @word "b". @node en:invariant (0 #"a"). @node en:node0 (0 #"a", 1 #"a"). @node en:node1 (0 #"a", 1 #"b"). @paranode en:paranode (0 #"a", 1 #{"a";"b"}).)");

    try {
        parser.parse();
    } catch (std::exception& e) {
        EXPECT_TRUE(false) << e.what();
    }

    ASSERT_EQ(parser.getSyntaxErrors().size(), 0);


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
