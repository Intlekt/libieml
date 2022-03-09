#include "structure/script/ScriptRegister.h"


using namespace ieml::structure;


const MultiplicativeScript* ScriptRegister::get_or_create_multiplication(const MultiplicativeScript::Children& children) {
    auto it = multiplicative_scripts_.find(children);
    if (it == multiplicative_scripts_.end()) {
        const auto script = new MultiplicativeScript(*this, children);
        multiplicative_scripts_.insert({children, script});
        return script;
    } else {
        return it->second;
    }
}

const AdditiveScript* ScriptRegister::get_or_create_addition(const Script::Set& children) {
    if (children.size() == 0) 
        return nullptr;

    if (children.size() == 1)
        // cannot create an addition from only one element
        return nullptr;

    auto it = additive_scripts_.find(children);
    if (it == additive_scripts_.end()) {
        const auto script = new AdditiveScript(children);
        additive_scripts_.insert({children, script});
        return script;
    } else {
        return it->second;
    }
}

void ScriptRegister::_build_primitives() {
    primitives_ = std::unordered_map<char, const PrimitiveScript*>{
            {'E', new PrimitiveScript('E')},
            {'A', new PrimitiveScript('A')},
            {'U', new PrimitiveScript('U')},
            {'S', new PrimitiveScript('S')},
            {'B', new PrimitiveScript('B')},
            {'T', new PrimitiveScript('T')}
    };
} 


void ScriptRegister::_build_null_scripts() {
    null_scripts_ = {
            new NullScript(0),
            new NullScript(1),
            new NullScript(2),
            new NullScript(3),
            new NullScript(4),
            new NullScript(5), 
            new NullScript(6)
    };
} 


void ScriptRegister::_build_remarkable_multiplications() {
    remarkable_multiplications_ = std::unordered_map<std::string, const MultiplicativeScript*>{
        {"wo", get_or_create_multiplication({get_primitive('U'), get_primitive('U'), get_nullscript(0)})},
        {"wa", get_or_create_multiplication({get_primitive('U'), get_primitive('A'), get_nullscript(0)})},
        {"y", get_or_create_multiplication({get_primitive('U'), get_primitive('S'), get_nullscript(0)})},
        {"o", get_or_create_multiplication({get_primitive('U'), get_primitive('B'), get_nullscript(0)})},
        {"e", get_or_create_multiplication({get_primitive('U'), get_primitive('T'), get_nullscript(0)})},

        {"wu", get_or_create_multiplication({get_primitive('A'), get_primitive('U'), get_nullscript(0)})},
        {"we", get_or_create_multiplication({get_primitive('A'), get_primitive('A'), get_nullscript(0)})},
        {"u", get_or_create_multiplication({get_primitive('A'), get_primitive('S'), get_nullscript(0)})},
        {"a", get_or_create_multiplication({get_primitive('A'), get_primitive('B'), get_nullscript(0)})},
        {"i", get_or_create_multiplication({get_primitive('A'), get_primitive('T'), get_nullscript(0)})},

        {"j", get_or_create_multiplication({get_primitive('S'), get_primitive('U'), get_nullscript(0)})},
        {"g", get_or_create_multiplication({get_primitive('S'), get_primitive('A'), get_nullscript(0)})},
        {"s", get_or_create_multiplication({get_primitive('S'), get_primitive('S'), get_nullscript(0)})},
        {"b", get_or_create_multiplication({get_primitive('S'), get_primitive('B'), get_nullscript(0)})},
        {"t", get_or_create_multiplication({get_primitive('S'), get_primitive('T'), get_nullscript(0)})},

        {"h", get_or_create_multiplication({get_primitive('B'), get_primitive('U'), get_nullscript(0)})},
        {"c", get_or_create_multiplication({get_primitive('B'), get_primitive('A'), get_nullscript(0)})},
        {"k", get_or_create_multiplication({get_primitive('B'), get_primitive('S'), get_nullscript(0)})},
        {"m", get_or_create_multiplication({get_primitive('B'), get_primitive('B'), get_nullscript(0)})},
        {"n", get_or_create_multiplication({get_primitive('B'), get_primitive('T'), get_nullscript(0)})},

        {"p", get_or_create_multiplication({get_primitive('T'), get_primitive('U'), get_nullscript(0)})},
        {"x", get_or_create_multiplication({get_primitive('T'), get_primitive('A'), get_nullscript(0)})},
        {"d", get_or_create_multiplication({get_primitive('T'), get_primitive('S'), get_nullscript(0)})},
        {"f", get_or_create_multiplication({get_primitive('T'), get_primitive('B'), get_nullscript(0)})},
        {"l", get_or_create_multiplication({get_primitive('T'), get_primitive('T'), get_nullscript(0)})}
    };
}

void ScriptRegister::_build_remarkable_additions() {
    remarkable_additions_ = std::unordered_map<char, const AdditiveScript*>{
        {'O', get_or_create_addition({get_primitive('U'), get_primitive('A')})},
        {'M', get_or_create_addition({get_primitive('S'), get_primitive('B'), get_primitive('T')})},
        {'F', get_or_create_addition({get_primitive('U'), get_primitive('A'), get_primitive('S'), get_primitive('B'), get_primitive('T')})},
        {'I', get_or_create_addition({get_primitive('E'), get_primitive('U'), get_primitive('A'), get_primitive('S'), get_primitive('B'), get_primitive('T')})}
    };
}
