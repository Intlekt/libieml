#include "structure/script/AdditiveScript.h"
#include "structure/script/PrimitiveScript.h"



using namespace ieml::structure;



std::string AdditiveScript::string_repr(const Script::Set& children) {
    std::stringstream os;
    bool first = true;
    for (auto& c : children) {
        if (first) first = false;
        else       os << "+";
        os << c->to_string();
    }
    const auto key = os.str();

    auto it = AdditiveScript::REMARKABLE_ADDITIONS_STRINGS.find(key);
    if (it != AdditiveScript::REMARKABLE_ADDITIONS_STRINGS.end()) return it->second;
    else return key;
};

std::u16string AdditiveScript::_canonical(const Script::Set& children) {
    if ((*children.begin())->get_layer() == 0) {
        std::u16string res(1, 0);

        for (auto& c : children)
            res[0] = res[0] | c->get_canonical()[0];

        return res;
    }
    size_t size = (*children.begin())->get_canonical().size() * 3;

    std::u16string res;
    res.reserve(size * 2); // u16 string

    for (auto& c : children)
        res = res.append(c->get_canonical());
    
    return res;
};

size_t AdditiveScript::_multiplicity(const Script::Set& children) {
    size_t res = 0;
    for (auto& c : children) res += c->get_multiplicity();
    return res;
}

Script::Set AdditiveScript::_build_singular_sequences() const {
    Script::Set res;
    for (auto& c : children_) {
        const auto& ss = c->singular_sequences();
        res.insert(ss.begin(), ss.end());
    }

    return res;
}

const std::unordered_map<std::string, std::string> AdditiveScript::REMARKABLE_ADDITIONS_STRINGS = {
    {"U:+A:",             "O:"},
    {"S:+B:+T:",          "M:"},
    {"U:+A:+S:+B:+T:",    "F:"},
    {"E:+U:+A:+S:+B:+T:", "I:"}
};
