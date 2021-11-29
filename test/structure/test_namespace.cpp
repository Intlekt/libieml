#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>

#include "gtest/gtest.h"

#include "structure/Namespace.h"
#include "structure/Phrase.h"
#include "structure/LanguageString.h"


using namespace ieml::structure;
using namespace std;

TEST(ieml_structure_test_case, namespace) {
    Namespace<Phrase> n;

    LanguageString l = LanguageString(LanguageType::FR, "test");
    std::shared_ptr<Phrase> p = std::make_shared<Phrase>(std::set<shared_ptr<PathTree>>{});

    n.define(
        std::make_shared<Name>(unordered_set<LanguageString>{l}),
        p
    );

    std::shared_ptr<Phrase> res = n.resolve(l);

    EXPECT_EQ(res, p);
}