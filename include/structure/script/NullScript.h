#pragma once

#include "structure/script/Script.h"

#include <memory>


namespace ieml::structure {

class ScriptRegister;

class NullScript: public Script {
public:
    friend class ScriptRegister;

    virtual bool is_nullscript() const override {return true;};
    
private:
    NullScript(size_t layer) : 
        Script(
            layer, 
            "", 
            _canonical(layer), 
            1
        ) {
            singular_sequences_ = {this};
        }

    static std::u16string _canonical(size_t layer);
};
    
}
