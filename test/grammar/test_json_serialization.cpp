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
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;


TEST(ieml_grammar_test_case, json_serialization) {
    for (const auto& file_path : fs::directory_iterator(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER)) {
        std::ifstream file;
        file.open (file_path.path());

        std::string line;
        std::stringstream o;
        if (file.is_open()) {
            while (getline (file, line)) {
                o << line << '\n';
            }
            file.close();
        }

        auto parser = ieml::parser::IEMLParser({"default"}, {o.str()});
        parser.parse();

        auto res = ieml::parser::parserToJson(parser);

        // check that all the id that are defined in the json are present

        for (nlohmann::json::iterator it = res["elements"].begin(); it != res["elements"].end(); ++it) {
            auto v = it.value();
            EXPECT_EQ(it.key(), v["id"]) << "Key not equal with the element id for key" + std::string(it.key()) + " in file " + std::string(file_path.path());

            std::string file_id = v["range"]["file_id"];
            EXPECT_EQ(file_id, std::string("default"));
            
            EXPECT_TRUE(v.contains("translations")) << "Missing translations of " + it.key() + " in file " + std::string(file_path.path());
            EXPECT_TRUE(v["translations"].contains("EN")) << "Missing EN translations of " + it.key() + " in file " + std::string(file_path.path());
            EXPECT_TRUE(v["translations"].contains("FR")) << "Missing FR translations of " + it.key() + " in file " + std::string(file_path.path());

            if (v["type"] == "CATEGORY") {

                for (auto ref: v["back_references"])
                    EXPECT_TRUE(res["elements"].contains(ref)) << "Not containing back_references " + std::string(ref) + " of " + it.key() + " in file " + std::string(file_path.path());
                for (auto ref: v["references"])
                    EXPECT_TRUE(res["elements"].contains(ref)) << "Not containing references " + std::string(ref) + " of " + it.key() + " in file " + std::string(file_path.path());
                for (auto ref: v["singular_sequences"])
                    EXPECT_TRUE(res["elements"].contains(ref)) << "Not containing singular_sequences " + std::string(ref) + " of " + it.key() + " in file " + std::string(file_path.path());

                EXPECT_TRUE(res["elements"].contains(v["invariant"])) << "Not containing invariant " + std::string(v["invariant"]) + " of " + it.key() + " in file " + std::string(file_path.path());

                for (auto ref: v["paradigms"])
                    EXPECT_TRUE(res["elements"].contains(ref)) << "Not containing paradigms " + std::string(ref) + " of " + it.key() + " in file " + std::string(file_path.path());

                EXPECT_LE(v["nDimension"], 3);
            }
        }

        // check that the tables have valid ids
        for (nlohmann::json::iterator it = res["tables"].begin(); it != res["tables"].end(); ++it) {
            auto v = it.value();
            EXPECT_TRUE(res["elements"].contains(v["root"])) << "Not table root id " + std::string(v["root"]) + " of " + it.key() + " in file " + std::string(file_path.path());

               
        }
    }
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
    ieml::relation::buildCompositionRelationGraph(graph, parser.getContext()->getPathTreeRegister(), cregister, wregister);
    auto json = ieml::parser::binaryGraphToJson(graph, cregister, wregister, mapping).dump();
}

TEST(ieml_grammar_test_case, unique_id_pathtree) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::PathTree> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@word "a.". @inflection en:noun class:VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                              
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@word "a.". @inflection en:noun class:VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                 
        h1 = p1->uid();
    }

    EXPECT_EQ(h0, h1);
}

TEST(ieml_grammar_test_case, unique_id_word) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::Word> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@inflection en:noun class:VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@inflection en:noun class:VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        h1 = p1->uid();
    }

    EXPECT_EQ(h0, h1);
}