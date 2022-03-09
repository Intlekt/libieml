#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

#include "structure/script/Script.h"
#include "structure/script/NullScript.h"

#include "utils.h"


namespace ieml::structure {

class ScriptRegister;

class MultiplicativeScript : public Script {
public:
    friend class ScriptRegister;

    typedef std::tuple<const Script*, const Script*, const Script*> Children; 

    struct HashChildrenFunctor {
        size_t operator()(const Children& children) const {
            size_t res = 0;
            hash_combine(res, std::get<0>(children));
            hash_combine(res, std::get<1>(children));
            hash_combine(res, std::get<2>(children));
            return res;
        }
    };

    static const std::unordered_map<std::string, std::string> REMARKABLE_MULTIPLICATIONS_STRINGS;

private:
    MultiplicativeScript(ScriptRegister& reg, const Children& children) : 
        Script(
            std::get<0>(children)->get_layer() + 1, 
            string_repr(children), 
            _canonical(children),
            _multiplicity(children)
        ),
        substance_(std::get<0>(children)), 
        attribute_(std::get<1>(children)),
        mode_(std::get<2>(children)) {
            singular_sequences_ = _build_singular_sequences(reg);
        }

    const Script* substance_;
    const Script* attribute_;
    const Script* mode_;

    static std::string string_repr(const Children& children);
    static std::u16string _canonical(const Children& children);
    static size_t _multiplicity(const Children& children);

    Script::Set _build_singular_sequences(ScriptRegister&) const;

};

}