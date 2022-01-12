#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include "IemlParser.h"
#include "ParserJsonSerializer.h"

using namespace ieml;


int main(int , const char **) {
  std::ifstream file;
  file.open ("/home/louis/code/ieml/libieml/assets/examples/ieml-grammar.ieml");
  std::stringstream os;
  std::string line;
  while ( getline (file,line) )
  {
    os << line << '\n';
  }
  file.close();

  parser::IEMLParser parser(os.str());
  parser.parse();
  auto errors = parser.getSyntaxErrors();
  for ( auto it = errors.crbegin(); it != errors.crend(); ++it) {
    std::cout << (*it)->to_string() << std::endl;
  }


  return 0;
}
