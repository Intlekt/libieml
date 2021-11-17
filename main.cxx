#include <iostream>

#include "IemlParser.h"

using namespace ieml;

int main(int , const char **) {
  std::string input = "@component fr\"le chien mange du paté sur le tapis\" en\"the dog eats paté on the carpet\" (0 #test, 1 &et [~singulier #chien ~pluriel #chat ]) .";
  parser::IEMLParser parser(input);
  parser.parse();

  std::cout << "Input:" << std::endl;
  std::cout << input << std::endl << std::endl;

  std::cout << "Ouput:" << std::endl;
  std::cout << parser.getASTString() << std::endl;

  std::cout << std::endl << "Errors:" << std::endl;

  for (auto s : parser.getSyntaxErrors()) {
    std::cout << s->to_string() << std::endl;
  }

  return 0;
}
