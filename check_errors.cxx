#include <iostream>
#include <string>
#include <sstream>

#include "IemlParser.h"

using namespace ieml;


int main(int , const char **) {
  std::ostringstream os;
  while(std::cin) {
      std::string input_line;
      getline(std::cin, input_line);
      os << input_line << std::endl;
  };

  parser::IEMLParser parser(os.str());
  parser.parse();
  
  std::cout << parser.toJson().dump() << std::endl;
  return 0;
}
