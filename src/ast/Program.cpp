#include "ast/Program.h"

#include <sstream>


namespace ieml::AST {

std::string Program::to_string() const {
    std::ostringstream os;
    for (auto && declaration: declarations_) {
        os << declaration->to_string() << std::endl;
    }
    return os.str();   
}

}