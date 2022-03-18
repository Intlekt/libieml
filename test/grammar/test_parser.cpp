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
TEST(ieml_grammar_test_case, no_crash_erroneous_files) {
  if (fs::exists(TEST_IEML_ERRONEOUS_FILES_FOLDER)) {
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
}
TEST(ieml_grammar_test_case, language_per_file) {

  IEMLParser parser(std::vector<std::string>{"file_0", "file_1"}, 
                    std::vector<std::string>{R"(@language fr. @rootparadigm type:inflection "O:M:.". @word "we.". @inflection fr:nom en:noun class:VERB "a.". @node en:test (0 ~nom #"we.").)", 
                                             R"(@language en. @node en: test2 (0 ~noun #test).)"});
  try {
    parser.parse();
  } catch (std::exception& e) {
    EXPECT_TRUE(false) << e.what();
  }

  EXPECT_EQ(parser.getSyntaxErrors().size(), 0);
}

TEST(ieml_grammar_test_case, ensure_script_parser_not_raise_error) {
    ieml::parser::IEMLParser parser(R"(@rootparadigm type:inflection "asdsad".)");                           
    try {                                                           
      parser.parse();                                               
    } catch (std::exception& e) {                                  
      EXPECT_TRUE(false) << e.what();                              
    }                                                               
                                                                    
    std::ostringstream os;                                          
    for (auto& error : parser.getSyntaxErrors()) {                  
      os << error->to_string() << std::endl;                        
    }                                          
                       
    ASSERT_EQ(parser.getSyntaxErrors().size(), 1) << "Too much errors raised by the parser. "
                                                     "The script parser errors have to be ignored." << os.str();
}