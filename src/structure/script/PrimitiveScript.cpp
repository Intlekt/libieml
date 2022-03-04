#include "structure/script/PrimitiveScript.h"


using namespace ieml::structure;


const std::unordered_map<char, std::shared_ptr<PrimitiveScript>> PrimitiveScript::PRIMITIVES = {
    {'E', std::make_shared<PrimitiveScript>('E')},
    {'A', std::make_shared<PrimitiveScript>('A')},
    {'U', std::make_shared<PrimitiveScript>('U')},
    {'S', std::make_shared<PrimitiveScript>('S')},
    {'B', std::make_shared<PrimitiveScript>('B')},
    {'T', std::make_shared<PrimitiveScript>('T')}
};
