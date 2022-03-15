
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
    
    IEMLParser parser(R"(@rootparadigm type:CATEGORY "O:M:.". @rootparadigm type:INFLECTION "E:O:.". @inflection en:noun class:VERB "E:A:.". @node en:invalid node (0 ~invalid noun #"a."). @node en:valid node (0 ~noun #"a.").)");                                         
    try {                                                           
      parser.parse();                                              
    } catch (std::exception& e) {                                  
      EXPECT_TRUE(false) << e.what();                              
    }                                                               

    EXPECT_EQ(parser.getSyntaxErrors().size(), 1);
    
    auto node_valid = parser.getContext()->getCategoryRegister().resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::EN, "valid node"));
    EXPECT_NE(node_valid, nullptr);
}
