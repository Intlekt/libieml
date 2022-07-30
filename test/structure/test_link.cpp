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

TEST(ieml_structure_test_case, link_word_function_create_instance)
{
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

    const auto &ctx = parser.getContext();

    auto &refreg = ctx->getReferenceSchemaRegister();
    auto &linkreg = ctx->getLinkRegister();
    auto &creg = ctx->getCategoryRegister();
    auto &wreg = ctx->getWordRegister();
    auto &preg = ctx->getPathTreeRegister();

    const auto pt = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "link"));
    ASSERT_NE(pt, nullptr);

    const auto &link = linkreg.getLinkByPathTree(pt);
    ASSERT_NE(link, nullptr);

    ASSERT_TRUE(refreg.is_defined(pt));
    const auto &schema = refreg.get_schema(pt);

    const auto &refs = schema.getInstances();
    ASSERT_EQ(refs.size(), 4);

    for (const auto &r : refs)
    {
        const auto valuation = schema.valuation_from_reference_values(r);
        const auto name = link->getNameForValuation(preg, creg, wreg, valuation);
        ASSERT_NE(name.begin()->second.value(), "");
    }
}

TEST(ieml_structure_test_case, link_word_function_create_instance_with_literal)
{
    PARSE_NO_ERRORS(R"(
        @rootparadigm type:category "O:O:." .

        @rootparadigm type:inflection "M:".
        @inflection en: noun class:VERB "S:".

        @node en: wa (0 ~noun #"wa.").
        @node en: we (0 ~noun #"we.").
        @node en: wo (0 ~noun #"wo.").
        @node en: wu (0 ~noun #"wu.").

        @link
            args: ($A)
            en: link
            template-en: $A linked
            phraseWordInflection: ~noun
            (
                0 #"wa." <$A>
            )
        .

        @function 
            type:word 
            link:link 
            domain:($A in "O:O:.") 
            condition: $A.attribute == "U:"
        .
    )");

    const auto &ctx = parser.getContext();

    auto &refreg = ctx->getReferenceSchemaRegister();
    auto &linkreg = ctx->getLinkRegister();
    auto &creg = ctx->getCategoryRegister();
    auto &wreg = ctx->getWordRegister();
    auto &preg = ctx->getPathTreeRegister();

    const auto pt = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "link"));
    ASSERT_NE(pt, nullptr);

    ASSERT_TRUE(refreg.is_defined(pt));
    const auto &schema = refreg.get_schema(pt);

    const auto &refs = schema.getInstances();
    ASSERT_EQ(refs.size(), 2);
}

TEST(ieml_structure_test_case, link_word_auxiliary)
{
    PARSE_NO_ERRORS(R"(
        // E:.-F:.M:M:.-l.-'
        @rootparadigm type:auxiliary "E:.-F:.M:M:.-l.-'".

        @auxiliary 
            en:in
            role:6
            "E:.-T:.f.-l.-'".

        @auxiliary 
            fr:sous
            en:under
            role:6
            "E:.-U:.f.-l.-'".

        @auxiliary 
            fr:fond
            fr:base
            fr:cul
            en:bottom
            en:basis
            role:6
            "E:.-U:.l.-l.-'".

        @auxiliary 
            fr:vers l'avant
            en:ahead
            en:to the front
            role:6
            "E:.-A:.s.-l.-'".

        @word "A:".

        @rootparadigm type:inflection "M:".
        @inflection en: noun class:VERB "S:".

        @link
            args:($A, $B)
            en:contains
            template-en:$A est dans $B
            template-fr:$A is in $B
            phraseWordInflection: ~noun 
            
            (
                0 #"A:",
                1 #"A:" <$A>,
                8 #"A:" <$B>
            ).

        @function
            type:word
            link:contains
            domain:($A in "E:.-F:.M:M:.-l.-'", $B in "E:.-F:.M:M:.-l.-'")
            condition:
            (
                $A == $B
            ).
    )");

    const auto &ctx = parser.getContext();

    auto &refreg = ctx->getReferenceSchemaRegister();
    auto &linkreg = ctx->getLinkRegister();
    auto &creg = ctx->getCategoryRegister();
    auto &wreg = ctx->getWordRegister();
    auto &preg = ctx->getPathTreeRegister();

    const auto pt = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "contains"));
    ASSERT_NE(pt, nullptr);

    ASSERT_TRUE(refreg.is_defined(pt));
    const auto &schema = refreg.get_schema(pt);

    const auto &refs = schema.getInstances();
    ASSERT_EQ(refs.size(), 4); // 4 auxiliary of the paradigm has been declared
}