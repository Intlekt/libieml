#include "structure/script/NullScript.h"
#include "structure/script/PrimitiveScript.h"

#include <math.h>


using namespace ieml::structure;


std::u16string NullScript::_canonical(size_t layer) {
    return std::u16string(pow(3, layer), PrimitiveScript::CHAR_TO_CANONICAL.find('E')->second);
}
