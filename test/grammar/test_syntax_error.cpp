
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <exception>

#include "gtest/gtest.h"
#include "TestConfig.h"

#include "structure/LanguageString.h"

#include "IemlParser.h"
#include "ParserJsonSerializer.h"

using namespace ieml::parser;


TEST(ieml_grammar_test_case, syntax_error) {
    
    IEMLParser parser(R"(@word "a.". @inflection fr:noun VERB "E:A:.". @node fr:invalid node (0 ~invalid noun #"a."). @node fr:valid node (0 ~noun #"a.").)");                                         
    try {                                                           
      parser.parse();                                              
    } catch (std::exception& e) {                                  
      EXPECT_TRUE(false) << e.what();                              
    }                                                               

    EXPECT_EQ(parser.getSyntaxErrors().size(), 1);
    
    auto node_valid = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR, "valid node"));
    EXPECT_NE(node_valid, nullptr);
}
