#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"
#include "relation/Composition.h"
#include "relation/RelationGraph.h"
#include "ParserJsonSerializer.h"
#include "structure/path/PathTree.h"
#include "structure/LanguageString.h"

#include "utils_testing.h"

using namespace ieml::relation;

TEST(ieml_relation_test_case, test_relation_graph) {
    RelationGraph graph;
}
TEST(ieml_relation_test_case, test_graph_compilation_error_not_null) {
    ieml::parser::IEMLParser parser(R"(
@language fr.

@word "we." .
@word "wo." .
@word "wa." .

@junction fr:et "m." .

@inflection fr:singulier class:NOUN "a." .
@inflection fr:pluriel class:NOUN "b." .
@inflection fr:nom class:VERB "c." .

@component 
    fr:le chien mange du paté sur le tapis 
    en:the dog eats paté on the carpet
    (
        0 #( 0 ~nom #"wa."),
  // error =>      
        9,
        1 &et [
            ~singulier #( 0 ~nom #"we.")
            ~pluriel #( 0 ~nom #"wo.") 
        ]
    )
.
    )");                           
    try {                                                           
      parser.parse();                                               
    } catch (std::exception& e) {                                   
      EXPECT_TRUE(false) << e.what();                               
    }                                                               

    auto& wregister = parser.getContext()->getWordRegister();
    auto& cregister = parser.getContext()->getCategoryRegister();
    auto& mapping = parser.getContext()->getSourceMapping();
    ieml::relation::RelationGraph graph;
    ieml::relation::buildCompositionRelationGraph(graph, parser.getContext()->getPathTreeRegister(), cregister, wregister);
    auto result = ieml::parser::binaryGraphToJson(graph, cregister, wregister, mapping);

    EXPECT_NE(result["nodes"], nullptr);
    EXPECT_NE(result["relations"], nullptr);

}