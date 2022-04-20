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

TEST(ieml_structure_test_case, link_word_function_create_instance_with_literal_2)
{
    PARSE_NO_ERRORS(R"(
        // E:.-F:.M:M:.-l.-'
        @rootparadigm type:auxiliary "E:.-F:.M:M:.-l.-'".

        @auxiliary 
            fr:vers le haut
            fr:hauteur
            en:to the top
            en:height
            role:6
            "E:.-U:.s.-l.-'".

        @auxiliary 
            fr:sur
            fr:au dessus
            en:on
            en:above
            en:on top
            role:6
            "E:.-U:.b.-l.-'".

        @auxiliary 
            fr:sommet
            en:summit
            role:6
            "E:.-U:.t.-l.-'".

        @auxiliary 
            fr:horizontalement
            en:horizontally
            role:6
            "E:.-U:.k.-l.-'".

        @auxiliary 
            fr:au même niveau
            en:at the same level
            role:6
            "E:.-U:.m.-l.-'".

        @auxiliary 
            fr:à mi-hauteur
            en:at half-height
            role:6
            "E:.-U:.n.-l.-'".

        @auxiliary 
            fr:vers le bas
            fr:en profondeur
            en:downward
            en:in depth
            role:6
            "E:.-U:.d.-l.-'".

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

        @auxiliary 
            fr:devant
            en:in front of
            role:6
            "E:.-A:.b.-l.-'".

        @auxiliary 
            fr:face
            fr:antérieur
            en:face
            en:anterior
            role:6
            "E:.-A:.t.-l.-'".

        @auxiliary 
            fr:verticalement
            en:vertically
            role:6
            "E:.-A:.k.-l.-'".

        @auxiliary 
            fr:du même côté
            fr:cis
            en:on the same side
            en:cis
            role:6
            "E:.-A:.m.-l.-'".

        @auxiliary 
            fr:mi-chemin
            fr:milieu
            en:half way
            en:middle
            role:6
            "E:.-A:.n.-l.-'".

        @auxiliary 
            fr:vers l'arrière
            en:to the back
            role:6
            "E:.-A:.d.-l.-'".

        @auxiliary 
            fr:derrière
            en:behind
            role:6
            "E:.-A:.f.-l.-'".

        @auxiliary 
            fr:dos
            fr:postérieur
            en:back
            en:posterior
            role:6
            "E:.-A:.l.-l.-'".

        @auxiliary 
            fr:commencement
            fr:origine
            fr:source
            en:beginning
            en:origin
            en:source
            role:6
            "E:.-S:.s.-l.-'".

        @auxiliary 
            fr:point de départ
            en:starting point
            role:6
            "E:.-S:.b.-l.-'".

        @auxiliary 
            fr:point d'entrée
            en:entry point
            role:6
            "E:.-S:.t.-l.-'".

        @auxiliary 
            fr:mobile
            fr:continuant
            fr:ininterrompu
            en:mobile
            en:ongoing
            en:uninterrupted
            role:6
            "E:.-S:.k.-l.-'".

        @auxiliary 
            fr:trajet
            fr:en chemin
            en:en route
            en:on the way
            role:6
            "E:.-S:.m.-l.-'".

        @auxiliary 
            fr:voie - rôle
            fr:route - rôle
            fr:canal - rôle
            fr:tunnel - rôle
            en:way - role
            en:road - role
            en:channel - role
            en:tunnel - role
            role:6
            "E:.-S:.n.-l.-'".

        @auxiliary 
            fr:puits - rôle
            fr:terminaison - rôle
            fr:fin - rôle
            en:well - role
            en:termination - role
            en:end - role
            role:6
            "E:.-S:.d.-l.-'".

        @auxiliary 
            fr:point d'arrivée
            fr:destination
            en:arrival point
            en:destination
            role:6
            "E:.-S:.f.-l.-'".

        @auxiliary 
            fr:point de sortie
            en:point of exit
            role:6
            "E:.-S:.l.-l.-'".

        @auxiliary 
            fr:ouvert
            fr:ouverture
            en:open
            en:openness
            role:6
            "E:.-B:.s.-l.-'".

        @auxiliary 
            fr:à gauche
            en:to the left
            role:6
            "E:.-B:.b.-l.-'".

        @auxiliary 
            fr:centre
            fr:au centre
            en:center
            en:at the center
            role:6
            "E:.-B:.t.-l.-'".

        @auxiliary 
            fr:au bord
            fr:à la limite
            fr:sur le point de
            en:at the edge of
            en:to the limit
            en:on the verge of
            en:about to
            role:6
            "E:.-B:.k.-l.-'".

        @auxiliary 
            fr:entre
            en:between
            role:6
            "E:.-B:.m.-l.-'".

        @auxiliary 
            fr:près
            fr:ici
            fr:à
            en:near
            en:close by
            en:here
            en:at
            role:6
            "E:.-B:.n.-l.-'".

        @auxiliary 
            fr:fermeture
            fr:fermé
            en:closing
            en:closed
            role:6
            "E:.-B:.d.-l.-'".

        @auxiliary 
            fr:à droite
            en:to the right
            role:6
            "E:.-B:.f.-l.-'".

        @auxiliary 
            fr:là-bas
            fr:loin
            en:over there
            en:far away
            role:6
            "E:.-B:.l.-l.-'".

        @auxiliary 
            fr:à partir de
            en:from
            role:6
            "E:.-T:.s.-l.-'".

        @auxiliary 
            fr:hors de
            en:out of
            role:6
            "E:.-T:.b.-l.-'".

        @auxiliary 
            fr:centrifuge
            fr:périphérique
            en:centrifugal
            en:peripheral
            role:6
            "E:.-T:.t.-l.-'".

        @auxiliary 
            fr:par
            fr:via
            en:by
            en:via
            role:6
            "E:.-T:.k.-l.-'".

        @auxiliary 
            fr:à travers
            fr:trans
            en:through
            en:trans
            role:6
            "E:.-T:.m.-l.-'".

        @auxiliary 
            fr:autour
            en:around
            role:6
            "E:.-T:.n.-l.-'".

        @auxiliary 
            fr:en direction de
            fr:vers
            en:towards
            role:6
            "E:.-T:.d.-l.-'".

        @auxiliary 
            fr:dans
            fr:dedans
            en:in
            en:into
            en:inside
            role:6
            "E:.-T:.f.-l.-'".

        @auxiliary 
            fr:centripète
            en:centripetal
            role:6
            "E:.-T:.l.-l.-'".


        @link
            args:($A, $B)
            fr:contraire
            en:contrary
            template-en:$A est le contraire de $B
            template-fr:$A is the contrary of $B
            phraseWordInflection: ~noun 
            
            (
                0 #signifier,
                1 #word <$A>,
                8 *contrary #word <$B>
            ).

        @function
            type:word
            link:contrary
            domain:($A in "E:.-F:.M:M:.-l.-'", $B in "E:.-F:.M:M:.-l.-'")
            condition:
            (
                $A.attribute.attribute.attribute == $B.attribute.attribute.attribute 
                and
                $A.attribute.attribute.substance == "S:"
                and
                $A.attribute.attribute.substance == "T:"
            ).
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