#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/path/PathTree.h"
#include "structure/Constants.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, path_node_inflection_ordering) {
    auto a = InflectionPathNode({std::make_shared<InflectionWord>("we.", InflectionType::NOUN), std::make_shared<InflectionWord>("wa.", InflectionType::NOUN)});
    auto b = InflectionPathNode({std::make_shared<InflectionWord>("wa.", InflectionType::NOUN), std::make_shared<InflectionWord>("we.", InflectionType::NOUN)});
    
    EXPECT_EQ(a, b);
}

TEST(ieml_structure_test_case, path_node_cmp_hash) {
    {
        auto a = RootPathNode();
        auto b = RootPathNode();

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = RoleNumberPathNode(RoleType::ROOT);
        auto b = RoleNumberPathNode(RoleType::ROOT);
        auto c = RoleNumberPathNode(RoleType::CAUSE);

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
        {
        auto a = PhraseJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = PhraseJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto c = PhraseJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = PhraseJunctionIndexPathNode(0);
        auto b = PhraseJunctionIndexPathNode(0);
        auto c = PhraseJunctionIndexPathNode(1);

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto c = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = AuxiliaryJunctionIndexPathNode(0);
        auto b = AuxiliaryJunctionIndexPathNode(0);
        auto c = AuxiliaryJunctionIndexPathNode(1);

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = InflectionJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = InflectionJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto c = InflectionJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = InflectionJunctionIndexPathNode(0);
        auto b = InflectionJunctionIndexPathNode(0);
        auto c = InflectionJunctionIndexPathNode(1);

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto c = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = CategoryJunctionIndexPathNode(0);
        auto b = CategoryJunctionIndexPathNode(0);
        auto c = CategoryJunctionIndexPathNode(1);

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT));
        auto b = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT));
        auto c = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("we.", RoleType::ROOT));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = InflectionPathNode({std::make_shared<InflectionWord>("we.", InflectionType::NOUN)});
        auto b = InflectionPathNode({std::make_shared<InflectionWord>("we.", InflectionType::NOUN)});
        auto c = InflectionPathNode({std::make_shared<InflectionWord>("we.", InflectionType::NOUN), std::make_shared<InflectionWord>("wa.", InflectionType::NOUN)});

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
    {
        auto a = WordPathNode(std::make_shared<CategoryWord>("wa."));
        auto b = WordPathNode(std::make_shared<CategoryWord>("wa."));
        auto c = WordPathNode(std::make_shared<CategoryWord>("wo."));

        EXPECT_EQ(a.to_string(), b.to_string());
        EXPECT_EQ(std::hash<PathNode>{}(a), std::hash<PathNode>{}(b));
        EXPECT_EQ(a, b);
        EXPECT_LE(a, b);
        EXPECT_LE(b, a);
        EXPECT_GE(a, b);
        EXPECT_GE(b, a);

        EXPECT_NE(a.to_string(), c.to_string());
        EXPECT_NE(std::hash<PathNode>{}(a), std::hash<PathNode>{}(c));
        EXPECT_NE(a, c);
        EXPECT_LT(a, c);
        EXPECT_GT(c, a);
        EXPECT_LE(a, c);
        EXPECT_GE(c, a);
    }
}