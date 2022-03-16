#include "structure/script/AdditiveScript.h"
#include "structure/script/PrimitiveScript.h"
#include "structure/script/ScriptRegister.h"



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
}

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
}

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

AdditiveScript::TablePtr AdditiveScript::build_table_paradigm(ScriptRegister& reg) const {
    // if a variable in the addition is not a paradigm return a Table1d with all the variables
    bool all_variable = true;
    for (const auto& child: children_) {
        all_variable = all_variable && (child->get_multiplicity() != 1);
    }

    if (!all_variable) {
        const auto sseq = std::vector<const Script*>(singular_sequences_.begin(), singular_sequences_.end());
        return new TableNd_<const Script*, 1>(
            sseq, 
            std::array<size_t, 1>{get_multiplicity()},
            std::array<std::vector<const Script*>, 1>{sseq},
            this
        );
    }

    // if all children are paradigm, build a TableSet from them.
    TableSet<const Script*>::Children children_tables;
    for (const auto& child: children_) {
        for (const auto& sub_tables: reg.get_or_create_table(child)->get_sub_tables())
            children_tables.push_back(sub_tables);
    }

    return new TableSet<const Script*>(
        children_tables,
        this
    );
}

const std::unordered_map<std::string, std::string> AdditiveScript::REMARKABLE_ADDITIONS_STRINGS = {
    {"U:+A:",             "O:"},
    {"S:+B:+T:",          "M:"},
    {"U:+A:+S:+B:+T:",    "F:"},
    {"E:+U:+A:+S:+B:+T:", "I:"}
};
