#include <iostream>

#include "IemlParser.h"

using namespace ieml;

int main(int , const char **) {
  std::string input = "@component fr\"test test\" en\"test en\" (0 ~nom #wa, 1 *tre ~test     #e133) .";
  parser::IEMLParser parser(input);
  parser.parse();

  std::cout << parser.getParseString() << std::endl;

  std::cout << std::endl << "Errors:" << std::endl;

  for (auto s : parser.getSyntaxErrors()) {
    std::cout << s->to_str() << std::endl;
  }

  return 0;
}
