#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include "IemlParser.h"

using namespace ieml;


int main(int , const char **) {
  std::string input = R"(@component fr"test" en"test" (0 # ) .)";
  

  parser::IEMLParser parser(input);
  parser.parse();
  
  std::cout << parser.toJson().dump() << std::endl;

  // std::cout << "Input:" << std::endl;
  // std::cout << input << std::endl << std::endl;

  // std::cout << "Ouput:" << std::endl;
  // std::cout << parser.getASTString() << std::endl;

  // std::cout << std::endl << "Errors:" << std::endl;

  // for (auto s : parser.getSyntaxErrors()) {
  //   std::cout << s->to_string() << std::endl;
  // }

  return 0;
}
