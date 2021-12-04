#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"
#include "relation/Composition.h"
#include "relation/GraphJsonSerializer.h"
#include "structure/Path.h"
#include "structure/LanguageString.h"

#include "test_utils.h"

using namespace ieml::relation;

#define HAS_RELATION(src, tgt, path) {\
  auto ret = graph->equal_range(src); \
  bool found = false;\
  for (auto it=ret.first; it != ret.second; ++it) {\
    if (it->second->getObject() == tgt) {\
      found = true;\
      auto p = ieml::structure::Path::from_string(path, *context);\
      EXPECT_EQ(*it->second->getPath(), *p) << "rel_path=" << it->second->getPath()->to_string() << " expect=" << p->to_string() << std::endl;\
      EXPECT_EQ(it->second->getSubject(), src);\
      EXPECT_EQ(it->second->getObject(), tgt);\
      break;\
    }\
  }\
  EXPECT_TRUE(found) << "Relation not in graph";\
}



TEST(ieml_relation_test_case, basic_graph) {
  PARSE_NO_ERRORS(R"(@word 'wa.'. @inflection fr"nom" VERB 'e.'. @component fr"included" (0 ~nom #'wa.'). @component fr"container" (0 #(0 ~nom #'wa.')).@component fr"topcontainer" (0 #(0 #(0 ~nom #'wa.'))).)");

  std::shared_ptr<CompositionRelationGraph> graph;
  std::shared_ptr<ieml::parser::ParserContext> context;
  context = parser.getContext();
  auto category_register = context->getCategoryRegister();

  try {                                                           
      graph = CompositionRelationGraph::buildFromCategoryRegister(context->getCategoryRegister());
  } catch (std::exception& e) {                                   
    EXPECT_TRUE(false) << e.what();                               
  }

  EXPECT_EQ(graph->size(), 3);

  auto included = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR, "included"));
  auto container = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR,"container"));
  auto topcontainer = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR,"topcontainer"));

  HAS_RELATION(container, included, R"(/#/0)");
  HAS_RELATION(topcontainer, container, R"(/#/0)");
  HAS_RELATION(topcontainer, included, R"(/#/0/#/0)");
}

// TEST(ieml_relation_test_case, json_serialization) {
//   PARSE_NO_ERRORS(R"(@word 'wa.'. @inflection fr"nom" VERB 'e.'. @node fr"included" (0 ~nom #'wa.'). @component fr"container" (0 #(0 ~nom #'wa.')).@component fr"topcontainer" (0 #(0 #(0 ~nom #'wa.'))).)");

//   std::shared_ptr<CompositionRelationGraph> graph;
//   std::shared_ptr<ieml::parser::ParserContext> context;
//   context = parser.getContext();
//   auto category_register = context->getCategoryRegister();

//   try {                                                           
//       graph = CompositionRelationGraph::buildFromCategoryRegister(context->getCategoryRegister());
//   } catch (std::exception& e) {                                   
//     EXPECT_TRUE(false) << e.what();                               
//   }

//   auto json = graph_to_json(graph, category_register);
//   std::cout << json.dump() << std::endl;
  

// }