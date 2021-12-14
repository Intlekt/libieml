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