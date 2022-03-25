#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <iostream>

#include "gtest/gtest.h"
#include "utils_testing.h"

#include "structure/Namespace.h"
#include "structure/LanguageString.h"
#include "structure/path/PathTree.h"


using namespace ieml::structure;
using namespace std;

TEST(ieml_structure_test_case, link_word_function_create_instance) {
    PARSE_NO_ERRORS(R"(
        @rootparadigm type:category "O:O:." .

        @rootparadigm type:inflection "M:".
        @inflection en: noun class:VERB "S:".

        @node en: wa (0 ~noun #"wa.").
        @node en: we (0 ~noun #"we.").
        @node en: wo (0 ~noun #"wo.").
        @node en: wu (0 ~noun #"wu.").

        @link
            args: ($A, $B)
            en: link
            template-en: $A linked with $B
            phraseWordInflection: ~noun
            (
                0 #"wa." <$A>,
                1 #"wa." <$B>
            )
        .

        @function 
            type:word 
            link:link 
            domain:($A in "O:O:.", $B in "O:O:.") 
            condition: $A.substance == $B.attribute and $A.attribute == $B.substance
        .
    )");

    const auto& ctx = parser.getContext();

    auto& refreg = ctx->getReferenceSchemaRegister();
    auto& linkreg = ctx->getLinkRegister();
    auto& creg = ctx->getCategoryRegister();
    auto& wreg = ctx->getWordRegister();
    auto& preg = ctx->getPathTreeRegister();

    const auto pt = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "link"));
    ASSERT_NE(pt, nullptr);

    const auto& link = linkreg.getLinkByPathTree(pt);
    ASSERT_NE(link, nullptr);

    ASSERT_TRUE(refreg.is_defined(pt));
    const auto& schema = refreg.get_schema(pt);

    const auto& refs = schema.getInstances();
    ASSERT_EQ(refs.size(), 4);

    for (const auto& r : refs) {
        const auto valuation = schema.valuation_from_reference_values(r);
        const auto name = link->getNameForValuation(preg, creg, wreg, valuation);
        ASSERT_NE(name.begin()->second.value(),  "");
    }
}
