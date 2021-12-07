#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>

#include "gtest/gtest.h"

#include "structure/Namespace.h"
#include "structure/LanguageString.h"
#include "structure/Path.h"


using namespace ieml::structure;
using namespace std;

TEST(ieml_structure_test_case, namespace) {
    Namespace<PathTree> n;
    PathTree::Register reg;

    LanguageString l = LanguageString(LanguageType::FR, "test");
    auto p = reg.get_or_create(std::make_shared<RootPathNode>(), PathTree::Children{});

    n.define(
        std::make_shared<Name>(unordered_multiset<LanguageString>{l}),
        p
    );

    std::shared_ptr<PathTree> res = n.resolve(l);

    EXPECT_EQ(res, p);
}

TEST(ieml_structure_test_case, namespace_multiple_traduction) {
    Namespace<PathTree> n;
    PathTree::Register reg;

    LanguageString l0 = LanguageString(LanguageType::FR, "test0");
    LanguageString l1 = LanguageString(LanguageType::FR, "test1");


    std::vector<std::shared_ptr<Name>> names = {
        std::make_shared<Name>(unordered_multiset<LanguageString>{l0}),
        std::make_shared<Name>(unordered_multiset<LanguageString>{l1})
    };

    auto p = reg.get_or_create(std::make_shared<RootPathNode>(), PathTree::Children{});

    for (auto& name : names) {
        n.define(
            name,
            p
        );
    }

    std::shared_ptr<PathTree> res = n.resolve(l0);
    EXPECT_EQ(res, p);
    res = n.resolve(l1);
    EXPECT_EQ(res, p);
}