#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/Word.h"
#include "structure/Constants.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, word_comparison) {
    {
        auto a = CategoryWord("wa.");
        auto b = CategoryWord("we.");
        auto c = CategoryWord("wa.");

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = InflectionWord("wa.", InflectionType::NOUN);
        auto b = InflectionWord("we.", InflectionType::NOUN);
        auto c = InflectionWord("wa.", InflectionType::VERB);

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = AuxiliaryWord("wa.", RoleType::ROOT);
        auto b = AuxiliaryWord("we.", RoleType::ROOT);
        auto c = AuxiliaryWord("wa.", RoleType::CAUSE);

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = JunctionWord("wa.");
        auto b = JunctionWord("we.");
        auto c = JunctionWord("wa.");

        EXPECT_NE(a, b);
        EXPECT_LT(a, b);
        EXPECT_GT(b, a);

        EXPECT_EQ(a, c);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = CategoryWord("wa.");
        auto b = InflectionWord("wa.", InflectionType::NOUN);
        auto c = AuxiliaryWord("wa.", RoleType::ROOT);
        auto d = JunctionWord("wa.");
        
        EXPECT_NE(a, b);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
        EXPECT_NE(b, c);
        EXPECT_NE(b, d);
        EXPECT_NE(c, d);
    }
}