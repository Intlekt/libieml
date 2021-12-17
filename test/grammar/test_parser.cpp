#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"

#include "IemlParser.h"


#include "TestConfig.h"

using namespace ieml::parser;

namespace fs = std::filesystem;


TEST(ieml_grammar_test_case, validate_exemples) {
    for (const auto& file : fs::directory_iterator(TEST_IEML_GRAMMAR_FILES_FOLDER)) {
      std::ifstream exampleFile(file.path());
      std::string exampleString, line;
      while (std::getline(exampleFile, line)) {
        exampleString = exampleString + "\n" + line;
      }
      exampleFile.close();

      IEMLParser parser(exampleString);

      try {
        parser.parse();
      } catch (std::exception& e) {
        EXPECT_TRUE(false) << e.what();
      }


      std::ostringstream os;
      for (auto& error : parser.getSyntaxErrors()) {
        os << error->to_string() << std::endl;
      }

      EXPECT_EQ(parser.getSyntaxErrors().size(), 0) << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl
                                                    << "[FAILURE] " << file.path() 
                                                    << " got " << parser.getSyntaxErrors().size() << " errors :" << std::endl
                                                    << os.str() << std::endl
                                                    << "file :" << std::endl 
                                                    << exampleString << std::endl
                                                    << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

  }
}
TEST(ieml_grammar_test_case, no_crash_erroneous_files) {
  for (const auto& file : fs::directory_iterator(TEST_IEML_ERRONEOUS_FILES_FOLDER)) {
    std::ifstream exampleFile(file.path());
    std::string exampleString, line;
    while (std::getline(exampleFile, line)) {
      exampleString = exampleString + "\n" + line;
    }
    exampleFile.close();

    IEMLParser parser(exampleString);

    try {
      parser.parse();
    } catch (std::exception& e) {
      EXPECT_TRUE(false) << e.what();
    }
  }
}
