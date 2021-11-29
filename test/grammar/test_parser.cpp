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
    for (const auto& file : fs::directory_iterator(TEST_IEML_GRAMMAR_EXAMPLES_FOLDER)) {
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

TEST(ieml_grammar_test_case, invalid_context) {
  {
    IEMLParser parser(R"(@component rr"test" (0 ~noun #'wa.').)");
    try {
      parser.parse();
    } catch (std::exception& e) {
      EXPECT_TRUE(false) << e.what();
    }

    EXPECT_NE(parser.getSyntaxErrors().size(), 0);
  }

}
TEST(ieml_grammar_test_case, invalid_identifier) {
  {
    IEMLParser parser(R"(@component fr"test" (0 ~noun #a).)");
    try {
      parser.parse();
    } catch (std::exception& e) {
      EXPECT_TRUE(false) << e.what();
    }

    EXPECT_NE(parser.getSyntaxErrors().size(), 0);
  }
}
TEST(ieml_grammar_test_case, component_declaration) {
  {
    IEMLParser parser(R"(@component fr"test" (0 #'wa.'). @component fr"test2" (0 #test).)");
    try {
      parser.parse();
    } catch (std::exception& e) {
      EXPECT_TRUE(false) << e.what();
    }

    std::ostringstream os;
    for (auto& error : parser.getSyntaxErrors()) {
      os << error->to_string() << std::endl;
    }


    EXPECT_EQ(parser.getSyntaxErrors().size(), 0) << os.str();
  }
}
TEST(ieml_grammar_test_case, extra_comma_in_phrase_line) {
  {
    IEMLParser parser(R"(@component fr"test" (0 #(0 ~noun #'wa.'),) .)");
    try {
      parser.parse();
    } catch (std::exception& e) {
      EXPECT_TRUE(false) << e.what();
    }


    EXPECT_NE(parser.getSyntaxErrors().size(), 0);
  }
}