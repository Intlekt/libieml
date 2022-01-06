#pragma once

#include <string>
#include <memory>

#include "structure/Path.h"
#include "structure/Word.h"


namespace ieml {
namespace structure {


std::string hashElement(std::shared_ptr<ieml::structure::PathTree> element);
std::string hashElement(std::shared_ptr<ieml::structure::Word> element);

}
}