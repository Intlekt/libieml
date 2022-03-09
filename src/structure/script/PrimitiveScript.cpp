#include "structure/script/PrimitiveScript.h"


using namespace ieml::structure;


const std::unordered_map<char, uint16_t> PrimitiveScript::CHAR_TO_CANONICAL = {
    {'E', 0b00000001},
    {'U', 0b00000010},
    {'A', 0b00000100},
    {'S', 0b00001000},
    {'B', 0b00010000},
    {'T', 0b00100000}
};
