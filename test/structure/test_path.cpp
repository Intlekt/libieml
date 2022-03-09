#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/path/PathParser.h"
#include "structure/Constants.h"
#include "ParserContextManager.h"

#include "test_utils.h"


using namespace ieml::structure;

#define PARSE_PATH_VALID(s, ctx) {                                          \
    auto p = ieml::parser::parsePath(ctx, s, true);                         \
    EXPECT_NE(p, nullptr) << "Unable to parse path : " << s;                \
    if (p) {                                                                \
        EXPECT_TRUE(p->is_path());                                          \
        EXPECT_EQ(p->to_string_path(), s);                                  \
        EXPECT_NO_THROW(p->is_valid()) << "Invalid path parsed : " << s;    \
    }                                                                       \
}


TEST(ieml_structure_test_case, path_serialization) {
    PathTree::Register reg;

    std::set<std::shared_ptr<InflectionWord>> plr{std::make_shared<InflectionWord>("we.", InflectionType::NOUN)};
    std::set<std::shared_ptr<InflectionWord>> plr_sing{std::make_shared<InflectionWord>("we.", InflectionType::NOUN), std::make_shared<InflectionWord>("wa.", InflectionType::NOUN)};
    {
        auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
        auto path1 = reg.get_or_create(std::make_shared<InflectionPathNode>(plr), {path});
        auto path2 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path1});
        
        EXPECT_TRUE(path2->is_valid()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string_path(), R"(/0/~"we."/"wa.")") << "Invalid path serialization";
    }
    {
        auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
        auto path1 = reg.get_or_create(std::make_shared<InflectionPathNode>(plr_sing), {path});
        auto path2 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path1});

        EXPECT_TRUE(path2->is_valid()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path2->to_string_path(), R"(/0/~"wa."~"we."/"wa.")") << "Invalid path serialization";
    }
    {
        auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
        auto path1 = reg.get_or_create(std::make_shared<InflectionPathNode>(plr_sing), {path});
        auto path2 = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(
            std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), {path1});
        auto path3 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path2});

        EXPECT_TRUE(path3->is_valid()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path3->to_string_path(), R"(/0/*"wa."/~"wa."~"we."/"wa.")") << "Invalid path serialization";
    }
    {
        auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
        auto path1 = reg.get_or_create(std::make_shared<CategoryJunctionIndexPathNode>(0), {path});
        auto path2 = reg.get_or_create(std::make_shared<CategoryJunctionPathNode>(std::make_shared<JunctionWord>("E:E:A:.")), {path1});
        auto path3 = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), {path2});
        auto path4 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path3});

        EXPECT_TRUE(path4->is_valid()) << "Invalid argument should not have been thrown on path.";
        EXPECT_EQ(path4->to_string_path(), R"(/0/*"wa."/&"E:E:A:."/[0]/"wa.")") << "Invalid path serialization";
    }
    
}

TEST(ieml_structure_test_case, path_from_string) {
    ieml::parser::IEMLParserErrorListener error_listener;
    ieml::parser::ParserContextManager ctx(&error_listener);
    auto& wregister = ctx.getWordRegister();
    wregister.define_word(std::make_shared<CategoryWord>("wa."));

    PARSE_PATH_VALID(R"(/"wa.")", ctx);

    std::unordered_multiset<LanguageString> s{LanguageString(LanguageType::FR, "aux_a")};
    wregister.define_auxiliary(std::make_shared<Name>(s), std::make_shared<AuxiliaryWord>("a.", RoleType::ROOT));
    PARSE_PATH_VALID(R"(/*"a.")", ctx);
    PARSE_PATH_VALID("/0", ctx);
    PARSE_PATH_VALID(R"(/0/*"a."/"wa.")", ctx);
    PARSE_PATH_VALID(R"(/#/0/*"a."/"wa.")", ctx);

    EXPECT_EQ(*ieml::parser::parsePath(ctx, "/#/0"), *ieml::parser::parsePath(ctx, "/#/0"));
}

TEST(ieml_structure_test_case, path_invalid) {
    PathTree::Register reg;

    {
        auto root0 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
        auto root1 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {root0});

        EXPECT_FALSE(root1->is_valid()) << "Invalid argument not raised on /0/0";
    }
    {
        auto root0 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
        auto path1 = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), {root0});
        auto root1 = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path1});

        EXPECT_FALSE(root1->is_valid()) << "Invalid argument not raised on /0/*ABOVE/0";
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
        PathTree::Register reg;

        std::shared_ptr<PathTree> a;
        {
            auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
            auto path1 = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)), {path});
            a = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path1});
        }
        std::shared_ptr<PathTree> b;
        {
            auto path = reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")));
            auto path1 = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("we.", RoleType::ROOT)), {path});
            b = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), {path1});
        }

        auto pathTree = reg.buildFromPaths({a, b});

        EXPECT_EQ(*pathTree->getNode(), *a->getNode());
        EXPECT_EQ(*pathTree->getNode(), *b->getNode());

        EXPECT_EQ(*pathTree->getChildrenAsVector()[0]->getNode(), *a->getChildrenAsVector()[0]->getNode());
        EXPECT_EQ(*pathTree->getChildrenAsVector()[1]->getNode(), *b->getChildrenAsVector()[0]->getNode());

        EXPECT_EQ(*pathTree->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getNode(), *a->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getNode());
        EXPECT_EQ(*pathTree->getChildrenAsVector()[1]->getChildrenAsVector()[0]->getNode(), *b->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getNode());
    }
}

TEST(ieml_structure_test_case, path_tree_building_error) {
    PathTree::Register reg;
    {
        auto a = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
        auto b = reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)));
        EXPECT_THROW(reg.buildFromPaths({a, b}), std::invalid_argument);
    }
    {

        auto a = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            {reg.get_or_create(std::make_shared<AuxiliaryPathNode>(std::make_shared<AuxiliaryWord>("wa.", RoleType::ROOT)))});

        auto b = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT), 
            {reg.get_or_create(std::make_shared<WordPathNode>(std::make_shared<CategoryWord>("wa.")))});
        
        EXPECT_THROW(reg.buildFromPaths({a, b}), std::invalid_argument);
    }

}

TEST(ieml_structure_test_case, path_tree_register) {
    PathTree::Register reg;

    auto a = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));
    auto b = reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT));

    EXPECT_EQ(a, b);

    auto c = reg.get_or_create(std::make_shared<RootPathNode>(), {reg.get_or_create(std::make_shared<RoleNumberPathNode>(RoleType::ROOT))});
    EXPECT_EQ(b, c->getChildrenAsVector()[0]);

    {
        PARSE_NO_ERRORS(R"(@word "wa.". @inflection en:noun class:VERB "e.". @component en:included (0 ~noun #"wa."). @component en:container (0 #(0 ~noun #"wa.")).)");
        auto context = parser.getContext();

        auto included = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::EN, "included"));
        auto container = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::EN, "container"));

        ASSERT_NE(included, nullptr);
        ASSERT_NE(container, nullptr);
        // assert that container['/#/0/#/0/~'e.'] == included['/#/0/~'e.']
        // value
        EXPECT_EQ(*container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], *included->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]);
        // pointer
        EXPECT_EQ(container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], included->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]);
 
        // assert that container['/#/0/#/0'] == included['/#/0']
        // value
        EXPECT_EQ(*container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], *included->getChildrenAsVector()[0]->getChildrenAsVector()[0]);
        // pointer
        EXPECT_EQ(container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], included->getChildrenAsVector()[0]->getChildrenAsVector()[0]);

        // assert that container['/#/0/#'] == included['/#']
        // value
        EXPECT_EQ(*container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], *included->getChildrenAsVector()[0]);
        // pointer
        EXPECT_EQ(container->getChildrenAsVector()[0]->getChildrenAsVector()[0]->getChildrenAsVector()[0], included->getChildrenAsVector()[0]);

        // assert that container['/#/0'] == included
        // value
        EXPECT_EQ(*container->getChildrenAsVector()[0]->getChildrenAsVector()[0], *included);
        // pointer
        EXPECT_EQ(container->getChildrenAsVector()[0]->getChildrenAsVector()[0], included);
    }
}


TEST(ieml_structure_test_case, path_tree_invariant) {
    PARSE_NO_ERRORS(R"(@word "wa.". @word "a.". @word "b.". @node en:root (0 #"wa."). @node en:node0 (0 #"wa.", 1 #"a."). @node en:node1 (0 #"wa.", 1 #"b.").)");
    auto context = parser.getContext();
    
    auto root = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::EN, "root"));
    auto node0 = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::EN, "node0"));
    auto node1 = context->getCategoryRegister().resolve_category(LanguageString(LanguageType::EN, "node1"));

    ASSERT_NE(root, nullptr);
    ASSERT_NE(node0, nullptr);
    ASSERT_NE(node1, nullptr);

    auto reg = context->getPathTreeRegister();

    auto paradigm = reg.get_or_create(std::make_shared<ParadigmPathNode>(), 
            {reg.get_or_create(std::make_shared<ParadigmIndexPathNode>(0), {node0}),
             reg.get_or_create(std::make_shared<ParadigmIndexPathNode>(1), {node1})});

    ASSERT_NE(paradigm, nullptr);

    ASSERT_TRUE(paradigm->is_valid());
    ASSERT_TRUE(paradigm->is_paradigm());

    auto invariant = reg.buildFromPaths(reg.invariant_paths(paradigm));

    ASSERT_NE(invariant, nullptr);

    EXPECT_EQ(invariant, root);
}   

TEST(ieml_structure_test_case, path_prefix) {
    ieml::parser::IEMLParserErrorListener error_listener;
    ieml::parser::ParserContextManager ctx(&error_listener);
    auto& wregister = ctx.getWordRegister();
    wregister.define_word(std::make_shared<CategoryWord>("wa."));


    auto p0 = ieml::parser::parsePath(ctx, R"(/#/0/"wa.")", true);   
    auto p1 = ieml::parser::parsePath(ctx, R"(/#/1/"wa.")", true);

    EXPECT_FALSE(p0->is_prefix(p1));


    p0 = ieml::parser::parsePath(ctx, R"(/#/0)", true);   
    p1 = ieml::parser::parsePath(ctx, R"(/#/1/"wa.")", true);

    EXPECT_FALSE(p0->is_prefix(p1));
}

TEST(ieml_structure_test_case, path_is_contained) {
    ieml::parser::IEMLParserErrorListener error_listener;
    ieml::parser::ParserContextManager ctx(&error_listener);
    auto& wregister = ctx.getWordRegister();
    wregister.define_word(std::make_shared<CategoryWord>("wa."));


    auto p0 = ieml::parser::parsePath(ctx, R"(/#/0/"wa.")", true);   
    auto p1 = ieml::parser::parsePath(ctx, R"(/#/1/"wa.")", true);

    EXPECT_FALSE(p0->is_contained(p1));


    p0 = ieml::parser::parsePath(ctx, R"(/#/0)", true);   
    p1 = ieml::parser::parsePath(ctx, R"(/#/1/"wa.")", true);

    EXPECT_FALSE(p0->is_contained(p1));
}
