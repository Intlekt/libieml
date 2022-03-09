#pragma once

#include <memory>
#include <set>
#include <sstream>

#include "structure/script/Script.h"
#include "structure/script/PrimitiveScript.h"


namespace ieml::structure {

class ScriptRegister;


class AdditiveScript : public Script {
public:
    friend class ScriptRegister;

    static const std::unordered_map<std::string, std::string> REMARKABLE_ADDITIONS_STRINGS;

private:
    AdditiveScript(const Script::Set& children) : 
        Script((*children.begin())->get_layer(), 
               string_repr(children), 
               _canonical(children), 
               _multiplicity(children)),
        children_(children) {
            singular_sequences_ = _build_singular_sequences();
        }

    const Script::Set children_;

    Script::Set _build_singular_sequences() const;

    static std::string string_repr(const Script::Set& children);
    static std::u16string _canonical(const Script::Set& children);
    static size_t _multiplicity(const Script::Set& children);
};

}