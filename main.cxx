#include <iostream>

#include "IemlParser.h"

using namespace ieml;

int main(int , const char **) {
  // std::string input;

  // getline(std::cin, input);

  std::string input = "@component fr\"test test\" pl\"test en\" (0 ~nom #wa) .";
  parser::IEMLParser parser(input);
  parser.parse();

  std::cout << parser.getParseString() << std::endl;

  std::cout << std::endl << "Errors:" << std::endl;

  for (auto s : parser.getSyntaxErrors()) {
    std::cout << s->to_str() << std::endl;
  }

  return 0;
}
