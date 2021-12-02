#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"
#include "relation/Composition.h"
#include "structure/Path.h"

#include "test_utils.h"

using namespace ieml::relation;



TEST(ieml_relation_test_case, basic_graph) {
  PARSE_NO_ERRORS(R"(@word 'wa.'. @inflection fr"nom" VERB 'e.'. @component fr"included" (0 ~nom #'wa.'). @component fr"container" (0 #(0 ~nom #'wa.')).)");

  std::shared_ptr<CompositionRelationGraph> graph;
  std::shared_ptr<ieml::parser::ParserContext> context;
  context = parser.getContext();

  try {                                                           
      graph = CompositionRelationGraph::buildFromCategoryRegister(*context);
  } catch (std::exception& e) {                                   
    EXPECT_TRUE(false) << e.what();                               
  }

  auto included = context->resolve_category("included");
  auto container = context->resolve_category("container");
  
  auto it = graph->find(container);
  ASSERT_NE(it, graph->end());

  EXPECT_EQ(graph->size(), 1);
  EXPECT_EQ(*it->second->getPath(), *ieml::structure::Path::from_string(R"(/#/0)", *context));
  EXPECT_EQ(it->second->getSubject(), included);
  EXPECT_EQ(it->second->getObject(), container);
}