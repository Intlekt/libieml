#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"
#include "relation/Inclusion.h"
#include "test_utils.h"

using namespace ieml::relation;


#define HAS_RELATION_INCLUSION(src, tgt) {\
  bool found = false;\
  boost::graph_traits<RelationGraph::Graph>::out_edge_iterator e, eend; \
  for (std::tie(e, eend) = boost::out_edges(src, graph); e != eend; ++e) {\
    if (graph[*e].attribute->getRelationType() == +RelationType::INCLUSION && boost::target(*e, graph) == tgt) {\
      found = true;\
      break;\
    }\
  }\
  EXPECT_TRUE(found) << "Relation not in graph";\
}


using namespace ieml::parser;


TEST(ieml_relation_test_case, inclusion) {
    PARSE_NO_ERRORS(R"(@word "a". @word "b". 
                       @node en:invariant (0 #"a"). 
                       @node en:node0 (0 #"a", 1 #"a"). 
                       @node en:node1 (0 #"a", 1 #"b"). 
                       @paranode en:paranode 1d:/#/1 (0 #"a", 1 {#"a";#"b"}).)");

    auto wreg = parser.getContext()->getWordRegister();
    auto creg = parser.getContext()->getCategoryRegister();

    const auto& paradigm = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "paranode"));
    const auto& node0 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node0"));
    const auto& node1 = creg.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "node1"));

    ASSERT_NE(paradigm, nullptr);
    ASSERT_NE(node0, nullptr);
    ASSERT_NE(node1, nullptr);

    ieml::relation::RelationGraph relgraph;
    ieml::relation::buildInclusionRelationGraph(relgraph, parser.getContext()->getPathTreeRegister(), creg, wreg);
    auto& node_register = relgraph.getRegister();
    auto& graph = relgraph.getGraph();

    HAS_RELATION_INCLUSION(node_register.get_or_create(paradigm, graph), node_register.get_or_create(node0, graph))
    HAS_RELATION_INCLUSION(node_register.get_or_create(paradigm, graph), node_register.get_or_create(node1, graph))
}