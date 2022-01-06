#include "structure/HashElement.h"


std::string ieml::structure::hashElement(std::shared_ptr<ieml::structure::PathTree> element) {
    auto hasher = std::hash<ieml::structure::PathTree>();
    return "category_" + std::to_string(hasher(*element));
}

std::string ieml::structure::hashElement(std::shared_ptr<ieml::structure::Word> element) {
    auto hasher = std::hash<std::string>();
    return "word_" + std::to_string(hasher(element->to_string()));
}