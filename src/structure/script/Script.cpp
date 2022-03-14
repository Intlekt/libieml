#include "structure/script/Script.h"
#include "structure/script/AdditiveScript.h"
#include "structure/script/MultiplicativeScript.h"
#include "structure/script/NullScript.h"
#include "structure/script/PrimitiveScript.h"


using namespace ieml::structure;


const Table<const Script*>* Script::get_table(ScriptRegister& reg) const {
    if (get_multiplicity() == 1)
        return new Cell<const Script*>(this);

    return build_table_paradigm(reg);
};


const char Script::LAYER_MARKS[] = {LAYER0_MARK, LAYER1_MARK, LAYER2_MARK, LAYER3_MARK, LAYER4_MARK, LAYER5_MARK, LAYER6_MARK};

const std::unordered_map<char, size_t> Script::LAYER_MARK_TO_LAYER = {
    {LAYER0_MARK, 0},
    {LAYER1_MARK, 1},
    {LAYER2_MARK, 2},
    {LAYER3_MARK, 3},
    {LAYER4_MARK, 4},
    {LAYER5_MARK, 5},
    {LAYER6_MARK, 6}
};


int Script::comp(const Script& a, const Script& b) {
    if (a.str_ == b.str_) return 0;

    const auto layer_a = a.get_layer();
    const auto layer_b = b.get_layer();
    
    if (layer_a != layer_b) 
        return layer_a < layer_b ? -1 : 1;

    // NullScript is minimal
    if (a.is_nullscript()) return -1;
    if (b.is_nullscript()) return 1;

    const auto multiplicity_a = a.get_multiplicity();
    const auto multiplicity_b = b.get_multiplicity();

    if (multiplicity_a != multiplicity_b) 
        return multiplicity_a < multiplicity_b ? -1 : 1;

    const auto diff_canonical = a.get_canonical().compare(b.get_canonical());
    if (diff_canonical != 0) return diff_canonical;

    throw std::runtime_error("Cannot compare scripts " + a.to_string() + " with " + b.to_string());
}
