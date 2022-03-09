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

private:
    PrimitiveScript(char c) : 
        Script(
            0, // layer
            std::string(1, c) + LAYER0_MARK, // string
            std::u16string(1, CHAR_TO_CANONICAL.find(c)->second), // canonical string
            1
        ), 
        char_(c) {
            singular_sequences_ = {this};
        }

    const char char_;
};


}