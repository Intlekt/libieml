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

    typedef const MultiplicativeScript* Ptr;

    typedef std::array<const Script*, 3> Children; 

    struct HashChildrenFunctor {
        size_t operator()(const Children& children) const {
            size_t res = 0;
            hash_combine(res, children[0]);
            hash_combine(res, children[1]);
            hash_combine(res, children[2]);
            return res;
        }
    };

    static const std::unordered_map<std::string, std::string> REMARKABLE_MULTIPLICATIONS_STRINGS;
    virtual ScriptType get_type() const override {return ScriptType::MULTIPLICATION;};

    Script::Ptr getSubstance() const {return children_[0];};
    Script::Ptr getAttribute() const {return children_[1];};
    Script::Ptr getMode() const {return children_[2];};

protected:
    virtual TablePtr build_table_paradigm(ScriptRegister&) const override;

private:
    MultiplicativeScript(ScriptRegister& reg, const Children& children) : 
        Script(
            children[0]->get_layer() + 1, 
            string_repr(children), 
            _canonical(children),
            _multiplicity(children)
        ),
        children_(children) {
            singular_sequences_ = _build_singular_sequences(reg);
        }

    const Children children_;

    static std::string string_repr(const Children& children);
    static std::u16string _canonical(const Children& children);
    static size_t _multiplicity(const Children& children);

    Script::Set _build_singular_sequences(ScriptRegister&) const;

};

}