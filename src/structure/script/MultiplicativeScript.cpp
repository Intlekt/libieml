#include "structure/script/MultiplicativeScript.h"
#include "structure/script/PrimitiveScript.h"
#include "structure/script/NullScript.h"
#include "structure/script/ScriptRegister.h"

#include <sstream>

using namespace ieml::structure;

std::string MultiplicativeScript::string_repr(const Children& children) {
    std::stringstream os;
    os << std::get<0>(children)->to_string();
    os << std::get<1>(children)->to_string();
    os << std::get<2>(children)->to_string();
    os << LAYER_MARKS[std::get<0>(children)->get_layer() + 1];

    const auto key = os.str();

    auto it = MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS.find(key);
    if (it != MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS.end()) return it->second;
    else return key;
};

std::u16string MultiplicativeScript::_canonical(const Children& children) {
    size_t size = std::get<0>(children)->get_canonical().size() * 3;

    std::u16string res;
    res.reserve(size * 2); // u16 string

    res = res.append(std::get<0>(children)->get_canonical());
    res = res.append(std::get<1>(children)->get_canonical());
    res = res.append(std::get<2>(children)->get_canonical());
    
    return res;
};

size_t MultiplicativeScript::_multiplicity(const Children& children) {
    const auto substance = std::get<0>(children);
    const auto attribute = std::get<1>(children);
    const auto mode      = std::get<2>(children);

    return substance->get_multiplicity() * attribute->get_multiplicity() * mode->get_multiplicity();
};

Script::Set MultiplicativeScript::_build_singular_sequences(ScriptRegister& reg) const {
    if (get_multiplicity() == 1)
        return {this};

    Script::Set res;
    for (const auto& ss_s: substance_->singular_sequences()) {
        for (const auto& ss_a: attribute_->singular_sequences()) {
            for (const auto& ss_m: mode_->singular_sequences()) {
                res.insert(reg.get_or_create_multiplication({ss_s, ss_a, ss_m}));
            }
        }
    }
    return res;
};

const std::unordered_map<std::string, std::string> MultiplicativeScript::REMARKABLE_MULTIPLICATIONS_STRINGS = {
    {"U:U:.", "wo."},
    {"U:A:.", "wa."},
    {"U:S:.", "y."},
    {"U:B:.", "o."},
    {"U:T:.", "e."},

    {"A:U:.", "wu."},
    {"A:A:.", "we."},
    {"A:S:.", "u."},
    {"A:B:.", "a."},
    {"A:T:.", "i."},

    {"S:U:.", "j."},
    {"S:A:.", "g."},
    {"S:S:.", "s."},
    {"S:B:.", "b."},
    {"S:T:.", "t."},

    {"B:U:.", "h."},
    {"B:A:.", "c."},
    {"B:S:.", "k."},
    {"B:B:.", "m."},
    {"B:T:.", "n."},

    {"T:U:.", "p."},
    {"T:A:.", "x."},
    {"T:S:.", "d."},
    {"T:B:.", "f."},
    {"T:T:.", "l."}
};