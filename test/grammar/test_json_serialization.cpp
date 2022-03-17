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

                ASSERT_TRUE(v.contains("table")) << "Missing table field for category";

                if (!v["table"].is_null()) {
                    ASSERT_TRUE(res["tables"].contains(v["table"])) << "Missing table " + std::string(v["table"]) + " of " + it.key() + " in file " + std::string(file_path.path());
                }
            }

            if (v["type"] == "SCRIPT") {
                ASSERT_TRUE(v.contains("str")) << "Missing str field for script";
                ASSERT_TRUE(v.contains("layer")) << "Missing layer field for script";
                ASSERT_TRUE(v.contains("multiplicity")) << "Missing multiplicity field for script";
                ASSERT_TRUE(v.contains("singular_sequences")) << "Missing singular_sequences field for script";
                ASSERT_TRUE(v.contains("script_type")) << "Missing script_type field for script";
                ASSERT_TRUE(v.contains("definition")) << "Missing definition field for script";
                ASSERT_TRUE(v.contains("declaration_type")) << "Missing declaration_type field for word";

                ASSERT_EQ(v["multiplicity"], v["singular_sequences"].size());

                for (auto ss : v["singular_sequences"])
                    EXPECT_TRUE(res["elements"].contains(ss)) << "Not containing singular sequence " + std::string(ss) + " of " + std::string(v["id"]) ;

                if (!v["definition"].is_null())
                    EXPECT_TRUE(res["elements"].contains(v["definition"])) << "Not containing word " + std::string(v["word"]) + " of " + std::string(v["id"]) ;

                ASSERT_TRUE(v.contains("table")) << "Missing table field for script";
                ASSERT_TRUE(res["tables"].contains(v["table"])) << "Missing table id in tables";
            }

            if (v["type"] == "WORD") {
                ASSERT_TRUE(v.contains("word_type")) << "Missing word_type field for word";
                ASSERT_TRUE(v.contains("declaration")) << "Missing declaration field for word";

                EXPECT_TRUE(res["elements"].contains(v["declaration"])) << "Not containing declaration " + std::string(v["declaration"]) + " of " + std::string(v["id"]) ;
            }
        }

        ASSERT_TRUE(res.contains("tables"));
        // check that the tables have valid ids
        for (nlohmann::json::iterator it = res["tables"].begin(); it != res["tables"].end(); ++it) {
            auto v = it.value();

            ASSERT_TRUE(v.contains("id")) << "Missing key id in table " + std::string(it.key());
            ASSERT_TRUE(res["tables"].contains(v["id"])) << "Table does not contains id " + std::string(it.key());
            
            ASSERT_TRUE(v.contains("title")) << "Missing key title in table " + std::string(it.key());
            ASSERT_TRUE(res["elements"].contains(v["title"])) << "No script id " + std::string(v["str"]) + " for table " + std::string(it.key()) + " in file " + std::string(file_path.path());

            ASSERT_TRUE(v.contains("element_type")) << "Missing key element_type in table " + std::string(it.key());
            ASSERT_TRUE(v.contains("type")) << "Missing key type in table " + std::string(it.key());
        
            if (v["type"] == "CELL") {}
            else if (v["type"] == "TABLEND") {
                ASSERT_TRUE(v.contains("n_dim")) << "Missing key n_dim in tablend " + std::string(it.key());
                ASSERT_TRUE(v.contains("shape")) << "Missing key shape in tablend " + std::string(it.key());

                ASSERT_EQ(v["shape"].size(), v["n_dim"]) << "Invalid dimension for shape in tablend " + std::string(it.key());

                ASSERT_TRUE(v.contains("headers")) << "Missing key headers in tablend " + std::string(it.key());
                ASSERT_EQ(v["headers"].size(), v["n_dim"]);
                for (size_t dim = 0; dim < v["n_dim"]; dim++) {
                    ASSERT_EQ(v["headers"][dim].size(), v["shape"][dim]);
                }

                ASSERT_TRUE(v.contains("cells")) << "Missing key cells in tablend " + std::string(it.key());

            } else if (v["type"] == "TABLESET") {
                ASSERT_TRUE(v.contains("children")) << "Missing key children in table " + std::string(it.key());
                for (auto c : v["children"]) {
                    ASSERT_TRUE(res["tables"].contains(c)) << "Table does not contains id " + std::string(c);
                }
            }
        
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
    auto& wregister = parser.getContext()->getWordRegister();
    auto& cregister = parser.getContext()->getCategoryRegister();
    auto& mapping = parser.getContext()->getSourceMapping();
    ieml::relation::buildCompositionRelationGraph(graph, parser.getContext()->getPathTreeRegister(), cregister, wregister);
    auto json = ieml::parser::binaryGraphToJson(graph, cregister, wregister, mapping).dump();
}

TEST(ieml_grammar_test_case, unique_id_pathtree) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::PathTree> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@rootparadigm type:CATEGORY "O:M:.". @rootparadigm type:INFLECTION "E:O:.". @inflection en:noun class:VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                              
        ASSERT_NE(p0, nullptr);
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@rootparadigm type:CATEGORY "O:M:.". @rootparadigm type:INFLECTION "E:O:.". @inflection en:noun class:VERB "E:A:.". @node en:valid node (0 ~noun #"a.").)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));                                 
        ASSERT_NE(p1, nullptr);
        h1 = p1->uid();
    }

    EXPECT_EQ(h0, h1);
}

TEST(ieml_grammar_test_case, unique_id_word) {
    std::string h0, h1;
    std::shared_ptr<ieml::structure::Word> p0, p1;
    {
        ieml::parser::IEMLParser parser(R"(@rootparadigm type:INFLECTION "E:O:.".@inflection en:noun class:VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }            
        p0 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        ASSERT_NE(p0, nullptr);
        h0 = p0->uid();
    }
    {
        ieml::parser::IEMLParser parser(R"(@rootparadigm type:INFLECTION "E:O:.". @inflection en:noun class:VERB "E:A:.".)");                                         
        try {                                                           
        parser.parse();                                              
        } catch (std::exception& e) {                                  
        EXPECT_TRUE(false) << e.what();                              
        }                              
        p1 = parser.getContext()->getWordRegister().resolve_inflection(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "noun"));                                              
        ASSERT_NE(p1, nullptr);
        h1 = p1->uid();
    }

    EXPECT_EQ(h0, h1);
}