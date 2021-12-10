#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include "IemlParser.h"
#include "relation/Composition.h"
#include "relation/GraphJsonSerializer.h"

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
  auto errors = parser.getSyntaxErrors();
  for ( auto it = errors.crbegin(); it != errors.crend(); ++it) {
    std::cout << (*it)->to_string() << std::endl;
  }



  if (errors.size() == 0) {

    auto cregister = parser.getContext()->getCategoryRegister();

    auto graph = ieml::relation::CompositionRelationGraph::buildFromCategoryRegister(cregister);

    std::cout << ieml::relation::graph_to_json(graph, cregister).dump() << std::endl;

  }


  return 0;
}
