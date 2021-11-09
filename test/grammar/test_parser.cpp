#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "gtest/gtest.h"

#include "IemlParser.h"


#include "TestConfig.h"

using namespace ieml::parser;

namespace fs = std::filesystem;


TEST(ieml_grammar_test_case, validate_exemples)
{
  for (const auto & file : fs::directory_iterator(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER)) {
    std::ifstream exampleFile(file.path());
    std::string exampleString, line;
    while (std::getline(exampleFile, line)) {
      exampleString = exampleString + "\n" + line;
    }
    exampleFile.close();

    IEMLParser parser(exampleString);
    parser.parse();
    std::cout << parser.getParseString() << std::endl << std::endl;

  }
}