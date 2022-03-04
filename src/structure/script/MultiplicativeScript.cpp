#include "structure/script/MultiplicativeScript.h"
#include "structure/script/PrimitiveScript.h"
#include "structure/script/NullScript.h"


using namespace ieml::structure;


const std::unordered_map<std::string, std::shared_ptr<MultiplicativeScript>> MultiplicativeScript::REMARKABLE_MULTIPLICATIONS = {
    {"wo", std::make_shared<MultiplicativeScript>(PRIMITIVE('U'), PRIMITIVE('U'), NullScript(0))},
    {"wa", std::make_shared<MultiplicativeScript>(PRIMITIVE('U'), PRIMITIVE('A'), NullScript(0))},
    {"y", std::make_shared<MultiplicativeScript>(PRIMITIVE('U'), PRIMITIVE('S'), NullScript(0))},
    {"o", std::make_shared<MultiplicativeScript>(PRIMITIVE('U'), PRIMITIVE('B'), NullScript(0))},
    {"e", std::make_shared<MultiplicativeScript>(PRIMITIVE('U'), PRIMITIVE('T'), NullScript(0))},

    {"wu", std::make_shared<MultiplicativeScript>(PRIMITIVE('A'), PRIMITIVE('U'), NullScript(0))},
    {"we", std::make_shared<MultiplicativeScript>(PRIMITIVE('A'), PRIMITIVE('A'), NullScript(0))},
    {"u", std::make_shared<MultiplicativeScript>(PRIMITIVE('A'), PRIMITIVE('S'), NullScript(0))},
    {"a", std::make_shared<MultiplicativeScript>(PRIMITIVE('A'), PRIMITIVE('B'), NullScript(0))},
    {"i", std::make_shared<MultiplicativeScript>(PRIMITIVE('A'), PRIMITIVE('T'), NullScript(0))},

    {"j", std::make_shared<MultiplicativeScript>(PRIMITIVE('S'), PRIMITIVE('U'), NullScript(0))},
    {"g", std::make_shared<MultiplicativeScript>(PRIMITIVE('S'), PRIMITIVE('A'), NullScript(0))},
    {"s", std::make_shared<MultiplicativeScript>(PRIMITIVE('S'), PRIMITIVE('S'), NullScript(0))},
    {"b", std::make_shared<MultiplicativeScript>(PRIMITIVE('S'), PRIMITIVE('B'), NullScript(0))},
    {"t", std::make_shared<MultiplicativeScript>(PRIMITIVE('S'), PRIMITIVE('T'), NullScript(0))},

    {"h", std::make_shared<MultiplicativeScript>(PRIMITIVE('B'), PRIMITIVE('U'), NullScript(0))},
    {"c", std::make_shared<MultiplicativeScript>(PRIMITIVE('B'), PRIMITIVE('A'), NullScript(0))},
    {"k", std::make_shared<MultiplicativeScript>(PRIMITIVE('B'), PRIMITIVE('S'), NullScript(0))},
    {"m", std::make_shared<MultiplicativeScript>(PRIMITIVE('B'), PRIMITIVE('B'), NullScript(0))},
    {"n", std::make_shared<MultiplicativeScript>(PRIMITIVE('B'), PRIMITIVE('T'), NullScript(0))},

    {"p", std::make_shared<MultiplicativeScript>(PRIMITIVE('T'), PRIMITIVE('U'), NullScript(0))},
    {"x", std::make_shared<MultiplicativeScript>(PRIMITIVE('T'), PRIMITIVE('A'), NullScript(0))},
    {"d", std::make_shared<MultiplicativeScript>(PRIMITIVE('T'), PRIMITIVE('S'), NullScript(0))},
    {"f", std::make_shared<MultiplicativeScript>(PRIMITIVE('T'), PRIMITIVE('B'), NullScript(0))},
    {"l", std::make_shared<MultiplicativeScript>(PRIMITIVE('T'), PRIMITIVE('T'), NullScript(0))}
};