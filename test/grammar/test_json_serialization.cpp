#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"
#include "TestConfig.h"

#include "IemlParser.h"
#include "ParserJsonSerializer.h"

#include "relation/Composition.h"


TEST(ieml_grammar_test_case, json_serialization) {
    std::ifstream file;
    file.open (std::string(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER) + "/ieml-grammar.ieml");

    std::string line;
    std::stringstream o;
    if (file.is_open()) {
        while (getline (file, line)) {
            o << line << '\n';
        }
        file.close();
    }

    auto parser = ieml::parser::IEMLParser(o.str());
    parser.parse();

    auto res = ieml::parser::parserToJson(parser);
}

TEST(ieml_grammar_test_case, composition_graph_json_serialization) {
    std::ifstream file;
    file.open (std::string(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER) + "/ieml-grammar.ieml");

    std::string line;
    std::stringstream o;
    if (file.is_open()) {
        while (getline (file, line)) {
            o << line << '\n';
        }
        file.close();
    }

    auto parser = ieml::parser::IEMLParser(o.str());
    parser.parse();
    
    ieml::relation::RelationGraph graph;
    auto wregister = parser.getContext()->getWordRegister();
    auto cregister = parser.getContext()->getCategoryRegister();
    auto mapping = parser.getContext()->getSourceMapping();
    ieml::relation::buildCompositionRelationGraph(graph, cregister, wregister);
    ieml::parser::binaryGraphToJson(graph, cregister, wregister, mapping).dump();
}

TEST(ieml_grammar_test_case, unique_id_pathtree) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::PathTree> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@word "a.". @inflection en:noun VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                              
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@word "a.". @inflection en:noun VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                 
        h1 = p1->uid();
    }

    EXPECT_EQ(*p0, *p1);
    EXPECT_EQ(h0, h1);
}

TEST(ieml_grammar_test_case, unique_id_word) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::Word> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@inflection en:noun VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@inflection en:noun VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        h1 = p1->uid();
    }

    EXPECT_EQ(*p0, *p1);
    EXPECT_EQ(h0, h1);
}