#include <iostream>

#include "IemlParser.h"

using namespace ieml;

int main(int , const char **) {
  std::string input;

  getline(std::cin, input);

  // std::string input = "@component tes fr\"test\" en\"voiture\" (0 ! ~rewrew #test <dt USL va (0 #test)>) .";
  parser::IEMLParser parser(input);
  parser.parse();

  std::cout << parser.getParseString() << std::endl;

  std::cout << std::endl << "Errors:" << std::endl;

  for (auto s : parser.getSyntaxErrors()) {
    std::cout << s->to_str() << std::endl;
  }

  return 0;
}
