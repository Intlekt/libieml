#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"
#include "relation/Composition.h"
#include "ParserJsonSerializer.h"
#include "structure/path/PathTree.h"
#include "structure/LanguageString.h"

#include "test_utils.h"

using namespace ieml::relation;

#define HAS_RELATION(src, tgt, path) {\
  bool found = false;\
  boost::graph_traits<RelationGraph::Graph>::out_edge_iterator e, eend; \
  for (std::tie(e, eend) = boost::out_edges(src, graph); e != eend; ++e) {\
    if (graph[*e].attribute->getRelationType() == +RelationType::COMPOSITION && boost::target(*e, graph) == tgt) {\
      found = true;\
      auto p = ieml::structure::Path::from_string(path, context->getWordRegister());\
      auto attr = std::dynamic_pointer_cast<CompositionAttribute>(graph[*e].attribute);\
      EXPECT_EQ(*attr->getCompositionPath(), *p) << "rel_path=" << attr->getCompositionPath()->to_string() << " expect=" << p->to_string() << std::endl;\
      break;\
    }\
  }\
  EXPECT_TRUE(found) << "Relation not in graph";\
}



TEST(ieml_relation_test_case, basic_graph) {
  PARSE_NO_ERRORS(R"(@word "wa.". @inflection en:noun VERB "e.". @component en:included (0 ~noun #"wa."). @component en:container (0 #(0 ~noun #"wa.")).@component en:topcontainer (0 #(0 #(0 ~noun #"wa."))).)");

  RelationGraph relgraph;
  std::shared_ptr<ieml::parser::ParserContextManager> context;
  context = parser.getContext();
  auto category_register = context->getCategoryRegister();
  auto& node_register = relgraph.getRegister();
  auto& graph = relgraph.getGraph();

  try {                                                           
      buildCompositionRelationGraph(relgraph, context->getCategoryRegister(), context->getWordRegister());
  } catch (std::exception& e) {                                   
    EXPECT_TRUE(false) << e.what();                               
  }
  auto included = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "included"));
  auto container = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN,"container"));
  auto topcontainer = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN,"topcontainer"));

  EXPECT_EQ(boost::num_edges(graph), 4);
  EXPECT_EQ(boost::num_vertices(graph), 5);
  HAS_RELATION(node_register.get_or_create(container, graph), node_register.get_or_create(included, graph), R"(/#/0)");
  HAS_RELATION(node_register.get_or_create(topcontainer, graph), node_register.get_or_create(container, graph), R"(/#/0)");

}
