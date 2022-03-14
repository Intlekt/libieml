#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"
#include "utils_testing.h"

#include "structure/Word.h"
#include "structure/Constants.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, word_comparison) {
    auto reg = ScriptRegister();

    {
        auto a = CategoryWord(ieml::testing::parse_script(&reg, "wa."));
        auto b = CategoryWord(ieml::testing::parse_script(&reg, "we."));
        auto c = CategoryWord(ieml::testing::parse_script(&reg, "wa."));

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = InflectionWord(ieml::testing::parse_script(&reg, "wa."), InflectionType::NOUN);
        auto b = InflectionWord(ieml::testing::parse_script(&reg, "we."), InflectionType::NOUN);
        auto c = InflectionWord(ieml::testing::parse_script(&reg, "wa."), InflectionType::VERB);

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = AuxiliaryWord(ieml::testing::parse_script(&reg, "wa."), RoleType::ROOT);
        auto b = AuxiliaryWord(ieml::testing::parse_script(&reg, "we."), RoleType::ROOT);
        auto c = AuxiliaryWord(ieml::testing::parse_script(&reg, "wa."), RoleType::CAUSE);

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = JunctionWord(ieml::testing::parse_script(&reg, "wa."));
        auto b = JunctionWord(ieml::testing::parse_script(&reg, "we."));
        auto c = JunctionWord(ieml::testing::parse_script(&reg, "wa."));

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = CategoryWord(ieml::testing::parse_script(&reg, "wa."));
        auto b = InflectionWord(ieml::testing::parse_script(&reg, "wa."), InflectionType::NOUN);
        auto c = AuxiliaryWord(ieml::testing::parse_script(&reg, "wa."), RoleType::ROOT);
        auto d = JunctionWord(ieml::testing::parse_script(&reg, "wa."));
        
        EXPECT_NE(a, b);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
        EXPECT_NE(b, c);
        EXPECT_NE(b, d);
        EXPECT_NE(c, d);
    }
}