#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>

#include "gtest/gtest.h"

#include "structure/Path.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, path_serialization) {

    std::set<std::shared_ptr<InflexingWord>> plr{std::make_shared<InflexingWord>("we.")};
    std::set<std::shared_ptr<InflexingWord>> plr_sing{std::make_shared<InflexingWord>("we."), std::make_shared<InflexingWord>("wa.")};
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
        EXPECT_EQ(path2->to_string(), "/0/~'we.','wa.'/'wa.'") << "Invalid path serialization";

    }
    std::unordered_set<RoleType> roles{RoleType::ROOT};
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr_sing), path);
        auto path2 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(
            std::make_shared<AuxiliaryWord>("wa.", roles)), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path2);

        EXPECT_NO_THROW(path3->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path3->to_string(), "/0/*'wa.'/~'we.','wa.'/'wa.'") << "Invalid path serialization";
    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<CategoryJunctionIndexPathNode>(0), path);
        auto path2 = std::make_shared<Path>(std::make_shared<CategoryJunctionPathNode>(JunctionType::AND), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", roles)), path2);
        auto path4 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path3);

        EXPECT_NO_THROW(path4->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path4->to_string(), "/0/*'wa.'/&AND/[0]/'wa.'") << "Invalid path serialization";
    }
    
}

TEST(ieml_structure_test_case, path_invalid) {
    std::unordered_set<RoleType> roles{RoleType::ROOT};

    {
        auto root0 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        auto root1 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), root0);

        EXPECT_THROW(root1->check(), std::invalid_argument) << "Invalid argument not raised on /0/0";
    }
    {
        auto root0 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", roles)), root0);
        auto root1 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);

        EXPECT_THROW(root1->check(), std::invalid_argument) << "Invalid argument not raised on /0/*ABOVE/0";
    }
}

TEST(ieml_structure_test_case, path_inflexing_ordering) {
    auto a = InflexingPathNode({std::make_shared<InflexingWord>("we."), std::make_shared<InflexingWord>("wa.")});
    auto b = InflexingPathNode({std::make_shared<InflexingWord>("wa."), std::make_shared<InflexingWord>("we.")});
    
    EXPECT_EQ(a, b);
}

TEST(ieml_structure_test_case, path_node_hash_eq) {
    std::unordered_set<RoleType> roles{RoleType::ROOT};

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
        auto a = AuxiliaryJunctionPathNode(JunctionType::AND);
        auto b = AuxiliaryJunctionPathNode(JunctionType::AND);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = AuxiliaryJunctionPathNode(JunctionType::AND);
        auto b = AuxiliaryJunctionPathNode(JunctionType::OR);

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
        auto a = InflexingJunctionPathNode(JunctionType::AND);
        auto b = InflexingJunctionPathNode(JunctionType::AND);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = InflexingJunctionPathNode(JunctionType::AND);
        auto b = InflexingJunctionPathNode(JunctionType::OR);

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
        auto a = CategoryJunctionPathNode(JunctionType::AND);
        auto b = CategoryJunctionPathNode(JunctionType::AND);

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = CategoryJunctionPathNode(JunctionType::AND);
        auto b = CategoryJunctionPathNode(JunctionType::OR);

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
        auto a = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", roles));
        auto b = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", roles));

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("we.", roles));
        auto b = AuxiliaryPathNode(std::make_shared<AuxiliaryWord>("wa.", roles));

        EXPECT_NE(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_NE(a, b);
    }
    {
        auto a = InflexingPathNode({std::make_shared<InflexingWord>("we.")});
        auto b = InflexingPathNode({std::make_shared<InflexingWord>("we.")});

        EXPECT_EQ(std::hash<PathNode*>{}(&a), std::hash<PathNode*>{}(&b));
        EXPECT_EQ(a, b);
    }
    {
        auto a = InflexingPathNode({std::make_shared<InflexingWord>("we.")});
        auto b = InflexingPathNode({std::make_shared<InflexingWord>("we."), std::make_shared<InflexingWord>("we.")});

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
    std::unordered_set<RoleType> roles{RoleType::ROOT};

    {
        
        std::shared_ptr<Path> a;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("we.", roles)), path);
            a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }
        std::shared_ptr<Path> b;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", roles)), path);
            b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }
        
        auto pathTree = PathTree::buildFromPaths({a, b});

        EXPECT_EQ(*pathTree->getNode(), *a->getNode());
        EXPECT_EQ(*pathTree->getNode(), *b->getNode());

        EXPECT_EQ(*pathTree->getChildren()[1]->getNode(), *a->getNext()->getNode());
        EXPECT_EQ(*pathTree->getChildren()[0]->getNode(), *b->getNext()->getNode());

        EXPECT_EQ(*pathTree->getChildren()[1]->getChildren()[0]->getNode(), *a->getNext()->getNext()->getNode());
        EXPECT_EQ(*pathTree->getChildren()[0]->getChildren()[0]->getNode(), *b->getNext()->getNext()->getNode());
    }
}

TEST(ieml_structure_test_case, path_tree_building_error) {
    std::unordered_set<RoleType> roles{RoleType::ROOT};

    {
        std::shared_ptr<Path> a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), nullptr);
        std::shared_ptr<Path> b = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", roles)), nullptr);
        
        EXPECT_THROW(PathTree::buildFromPaths({a, b}), std::invalid_argument);
    }
    {

        std::shared_ptr<Path> a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", roles)), nullptr));

        std::shared_ptr<Path> b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<Word>("wa.")), nullptr));
        
        EXPECT_THROW(PathTree::buildFromPaths({a, b}), std::invalid_argument);
    }

}
