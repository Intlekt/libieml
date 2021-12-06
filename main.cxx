#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include "IemlParser.h"

using namespace ieml;


int main(int , const char **) {
  std::ifstream file;
  file.open ("/home/louis/code/ieml/libieml/assets/exemples/ieml-grammar.ieml");
  std::stringstream os;
  std::string line;
  while ( getline (file,line) )
  {
    os << line << '\n';
  }
  file.close();
    

  parser::IEMLParser parser(os.str());
  parser.parse();
  
  std::cout << parser.toJson().dump() << std::endl;

  // auto context = parser.getContext();
  // auto category_register = context->getCategoryRegister();
  // auto p0 = structure::Path::from_string("/#/0", *context);
  // auto p1 = structure::Path::from_string("/#/0", *context);

  // assert(*p0 == *p1);
  // // auto included = context->resolve_category("included");
  // // auto container = context->resolve_category("container");

  // auto included = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR, "included"));
  // auto container = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR,"container"));
  // auto topcontainer = category_register.resolve_category(ieml::structure::LanguageString(ieml::structure::LanguageType::FR,"topcontainer"));


  return 0;
}
