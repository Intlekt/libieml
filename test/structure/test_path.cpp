#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>

#include "gtest/gtest.h"

#include "structure/Path.h"
#include "structure/Constants.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, path_serialization) {

    std::set<std::shared_ptr<InflexingWord>> plr{std::make_shared<InflexingWord>("we.", InflexingType::NOUN)};
    std::set<std::shared_ptr<InflexingWord>> plr_sing{std::make_shared<InflexingWord>("we.", InflexingType::NOUN), std::make_shared<InflexingWord>("wa.", InflexingType::NOUN)};
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr), path);
        auto path2 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        
        EXPECT_NO_THROW(path2->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string(), "/0/~'we.'/'wa.'") << "Invalid path serialization";
    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr_sing), path);
        auto path2 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);

        EXPECT_NO_THROW(path2->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string(), "/0/~'wa.','we.'/'wa.'") << "Invalid path serialization";

    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr_sing), path);
        auto path2 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(
            std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path2);

        EXPECT_NO_THROW(path3->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path3->to_string(), "/0/*'wa.'/~'wa.','we.'/'wa.'") << "Invalid path serialization";
    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<CategoryJunctionIndexPathNode>(0), path);
        auto path2 = std::make_shared<Path>(std::make_shared<CategoryJunctionPathNode>(std::make_shared<JunctionWord>("E:E:A:.")), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path2);
        auto path4 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path3);

        EXPECT_NO_THROW(path4->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path4->to_string(), "/0/*'wa.'/&'E:E:A:.'/[0]/'wa.'") << "Invalid path serialization";
    }
    
}

TEST(ieml_structure_test_case, path_invalid) {
    {
        auto root0 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        auto root1 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), root0);

        EXPECT_THROW(root1->check(), std::invalid_argument) << "Invalid argument not raised on /0/0";
    }
    {
        auto root0 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), root0);
        auto root1 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);

        EXPECT_THROW(root1->check(), std::invalid_argument) << "Invalid argument not raised on /0/*ABOVE/0";
    }
}

TEST(ieml_structure_test_case, path_inflexing_ordering) {
    auto a = InflexingPathNode({std::make_shared<InflexingWord>("we.", InflexingType::NOUN), std::make_shared<InflexingWord>("wa.", InflexingType::NOUN)});
    auto b = InflexingPathNode({std::make_shared<InflexingWord>("wa.", InflexingType::NOUN), std::make_shared<InflexingWord>("we.", InflexingType::NOUN)});
    
    EXPECT_EQ(a, b);
}

TEST(ieml_structure_test_case, path_node_hash_eq) {
    {
        auto a = RootPathNode();
        auto b = RootPathNode();

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = RoleNumberPathNode(RoleType::ROOT);
        auto b = RoleNumberPathNode(RoleType::ROOT);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = RoleNumberPathNode(RoleType::ROOT);
        auto b = RoleNumberPathNode(RoleType::CAUSE);

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = AuxiliaryJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = AuxiliaryJunctionIndexPathNode(0);
        auto b = AuxiliaryJunctionIndexPathNode(0);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = AuxiliaryJunctionIndexPathNode(1);
        auto b = AuxiliaryJunctionIndexPathNode(0);

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = InflexingJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = InflexingJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = InflexingJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = InflexingJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = InflexingJunctionIndexPathNode(0);
        auto b = InflexingJunctionIndexPathNode(0);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = InflexingJunctionIndexPathNode(1);
        auto b = InflexingJunctionIndexPathNode(0);

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:A:."));
        auto b = CategoryJunctionPathNode(std::make_shared<JunctionWord>("E:E:B:."));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = CategoryJunctionIndexPathNode(0);
        auto b = CategoryJunctionIndexPathNode(0);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = CategoryJunctionIndexPathNode(1);
        auto b = CategoryJunctionIndexPathNode(0);

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT));
        auto b = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("we.", RoleType::ROOT));
        auto b = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = InflexingPathNode({std::make_shared<InflexingWord>("we.", InflexingType::NOUN)});
        auto b = InflexingPathNode({std::make_shared<InflexingWord>("we.", InflexingType::NOUN)});

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = InflexingPathNode({std::make_shared<InflexingWord>("we.", InflexingType::NOUN)});
        auto b = InflexingPathNode({std::make_shared<InflexingWord>("we.", InflexingType::NOUN), std::make_shared<InflexingWord>("wa.", InflexingType::NOUN)});

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = WordPathNode(std::make_shared<Word>("wa."));
        auto b = WordPathNode(std::make_shared<Word>("wa."));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = WordPathNode(std::make_shared<Word>("wa."));
        auto b = WordPathNode(std::make_shared<Word>("wo."));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }

}

TEST(ieml_structure_test_case, path_tree_building) {
    {
        
        std::shared_ptr<Path> a;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path);
            a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }
        std::shared_ptr<Path> b;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("we.", RoleType::ROOT)), path);
            b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }
        
        auto pathTree = PathTree::buildFromPaths({a, b});

        EXPECT_EQ(*pathTree->getNode(), *a->getNode());
        EXPECT_EQ(*pathTree->getNode(), *b->getNode());

        EXPECT_EQ(*pathTree->getChildren()[0]->getNode(), *a->getNext()->getNode());
        EXPECT_EQ(*pathTree->getChildren()[1]->getNode(), *b->getNext()->getNode());

        EXPECT_EQ(*pathTree->getChildren()[0]->getChildren()[0]->getNode(), *a->getNext()->getNext()->getNode());
        EXPECT_EQ(*pathTree->getChildren()[1]->getChildren()[0]->getNode(), *b->getNext()->getNext()->getNode());
    }
}

TEST(ieml_structure_test_case, path_tree_building_error) {

    {
        std::shared_ptr<Path> a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        std::shared_ptr<Path> b = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), nullptr);
        
        EXPECT_THROW(PathTree::buildFromPaths({a, b}), std::invalid_argument);
    }
    {

        std::shared_ptr<Path> a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), nullptr));

        std::shared_ptr<Path> b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr));
        
        EXPECT_THROW(PathTree::buildFromPaths({a, b}), std::invalid_argument);
    }

}
