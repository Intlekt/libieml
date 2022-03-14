#pragma once

#include "structure/script/Script.h"


#include <unordered_map>
#include <memory>


namespace ieml::structure {

class ScriptRegister;

class PrimitiveScript: public Script {
public:
    friend class ScriptRegister;


    static const std::unordered_map<char, uint16_t> CHAR_TO_CANONICAL;

    virtual ScriptType get_type() const override {return ScriptType::PRIMITIVE;};

protected:
    virtual TablePtr build_table_paradigm(ScriptRegister&) const override {
        throw std::invalid_argument("A paradigm primitive is impossible.");
    }

private:
    PrimitiveScript(char c) : 
        Script(
            0, // layer
            std::string(1, c) + LAYER0_MARK, // string
            std::u16string(1, CHAR_TO_CANONICAL.find(c)->second), // canonical string
            1
        ) {
            singular_sequences_ = {this};
        }
};


}