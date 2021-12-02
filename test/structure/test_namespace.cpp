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
    auto p = reg.get_or_create(std::make_shared<RootPathNode>(), std::set<shared_ptr<PathTree>>{});

    n.define(
        std::make_shared<Name>(unordered_set<LanguageString>{l}),
        p
    );

    std::shared_ptr<PathTree> res = n.resolve(l);

    EXPECT_EQ(res, p);
}