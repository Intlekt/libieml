#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/Path.h"
#include "structure/Constants.h"
#include "ParserContext.h"

#include "test_utils.h"


using namespace ieml::structure;

#define PARSE_PATH_VALID(s, ctx) {                                      \
    auto p = Path::from_string(s, ctx);                                 \
    EXPECT_NE(p, nullptr) << "Unable to parse path : " << s;            \
    if (p) {                                                            \
        EXPECT_EQ(p->to_string(), s);                                   \
        EXPECT_NO_THROW(p->check()) << "Invalid path parsed : " << s;   \
    }                                                                   \
}


TEST(ieml_structure_test_case, path_serialization) {

    std::set<std::shared_ptr<InflexingWord>> plr{std::make_shared<InflexingWord>("we.", InflexingType::NOUN)};
    std::set<std::shared_ptr<InflexingWord>> plr_sing{std::make_shared<InflexingWord>("we.", InflexingType::NOUN), std::make_shared<InflexingWord>("wa.", InflexingType::NOUN)};
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr), path);
        auto path2 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        
        EXPECT_NO_THROW(path2->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string(), "/0/~'we.'/'wa.'") << "Invalid path serialization";
    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr_sing), path);
        auto path2 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);

        EXPECT_NO_THROW(path2->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string(), "/0/~'wa.'~'we.'/'wa.'") << "Invalid path serialization";

    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<InflexingPathNode>(plr_sing), path);
        auto path2 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(
            std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path2);

        EXPECT_NO_THROW(path3->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path3->to_string(), "/0/*'wa.'/~'wa.'~'we.'/'wa.'") << "Invalid path serialization";
    }
    {
        auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
        auto path1 = std::make_shared<Path>(std::make_shared<CategoryJunctionIndexPathNode>(0), path);
        auto path2 = std::make_shared<Path>(std::make_shared<CategoryJunctionPathNode>(std::make_shared<JunctionWord>("E:E:A:.")), path1);
        auto path3 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path2);
        auto path4 = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path3);

        EXPECT_NO_THROW(path4->check()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path4->to_string(), "/0/*'wa.'/&'E:E:A:.'/[0]/'wa.'") << "Invalid path serialization";
    }
    
}

TEST(ieml_structure_test_case, path_from_string) {
    ieml::parser::IEMLParserErrorListener error_listener;
    ieml::parser::ParserContext ctx(&error_listener);
    auto& wregister = ctx.getWordRegister();
    wregister.define_word(std::make_shared<CategoryWord>("wa."));

    PARSE_PATH_VALID("/'wa.'", wregister);

    std::unordered_multiset<LanguageString> s{LanguageString(LanguageType::FR, "aux_a")};
    wregister.define_auxiliary(std::make_shared<Name>(s), std::make_shared<AuxiliaryWord>("a.", RoleType::ROOT));
    PARSE_PATH_VALID("/*'a.'", wregister);
    PARSE_PATH_VALID("/0", wregister);
    PARSE_PATH_VALID("/0/*'a.'/'wa.'", wregister);
    PARSE_PATH_VALID("/#/0/*'a.'/'wa.'", wregister);


    EXPECT_EQ(*Path::from_string("/#/0", wregister), *Path::from_string("/#/0", wregister));
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


TEST(ieml_structure_test_case, path_tree_comparison) {
    PathTree::Register reg;

    auto tree1 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
    auto tree2 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::INITIATOR));
    auto tree3 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));

    EXPECT_EQ(*tree1, *tree3);
    EXPECT_NE(*tree1, *tree2);

    EXPECT_LE(*tree1, *tree3);
    EXPECT_LE(*tree1, *tree2);
    EXPECT_LT(*tree1, *tree2);
    
    EXPECT_GE(*tree1, *tree3);
    EXPECT_GE(*tree2, *tree1);
    EXPECT_GT(*tree2, *tree1);
}
TEST(ieml_structure_test_case, path_tree_building) {
    {

        std::shared_ptr<Path> a;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), path);
            a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }
        std::shared_ptr<Path> b;
        {
            auto path = std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr);
            auto path1 = std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("we.", RoleType::ROOT)), path);
            b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), path1);
        }

        PathTree::Register reg;
        auto pathTree = reg.buildFromPaths({a, b});

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
        PathTree::Register reg;
        EXPECT_THROW(reg.buildFromPaths({a, b}), std::invalid_argument);
    }
    {

        std::shared_ptr<Path> a = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), nullptr));

        std::shared_ptr<Path> b = std::make_shared<Path>(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            std::make_shared<Path>(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")), nullptr));
        
        PathTree::Register reg;
        EXPECT_THROW(reg.buildFromPaths({a, b}), std::invalid_argument);
    }

}

TEST(ieml_structure_test_case, path_tree_register) {
    PathTree::Register reg;

    auto a = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
    auto b = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));

    EXPECT_EQ(a, b);

    auto c = reg.get_or_create(std::make_shared<RootPathNode>(), {reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT))});
    EXPECT_EQ(b, c->getChildren()[0]);

    {
        PARSE_NO_ERRORS(R"(@word "wa.". @inflection fr'nom' VERB "e.". @component fr'included' (0 ~nom #"wa."). @component fr'container' (0 #(0 ~nom #"wa.")).)");
        auto context = parser.getContext();

        auto included = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::FR, "included"));
        auto container = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::FR, "container"));

        ASSERT_NE(included, nullptr);
        ASSERT_NE(container, nullptr);
        // assert that container['/#/0/#/0/~'e.'] == included['/#/0/~'e.']
        // value
        EXPECT_EQ(*container->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0], *included->getChildren()[0]->getChildren()[0]->getChildren()[0]);
        // pointer
        EXPECT_EQ(container->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0], included->getChildren()[0]->getChildren()[0]->getChildren()[0]);
 
        // assert that container['/#/0/#/0'] == included['/#/0']
        // value
        EXPECT_EQ(*container->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0], *included->getChildren()[0]->getChildren()[0]);
        // pointer
        EXPECT_EQ(container->getChildren()[0]->getChildren()[0]->getChildren()[0]->getChildren()[0], included->getChildren()[0]->getChildren()[0]);

        // assert that container['/#/0/#'] == included['/#']
        // value
        EXPECT_EQ(*container->getChildren()[0]->getChildren()[0]->getChildren()[0], *included->getChildren()[0]);
        // pointer
        EXPECT_EQ(container->getChildren()[0]->getChildren()[0]->getChildren()[0], included->getChildren()[0]);

        // assert that container['/#/0'] == included
        // value
        EXPECT_EQ(*container->getChildren()[0]->getChildren()[0], *included);
        // pointer
        EXPECT_EQ(container->getChildren()[0]->getChildren()[0], included);

    }
}
