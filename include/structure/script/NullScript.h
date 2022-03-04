#pragma once

#include "structure/script/Script.h"


namespace ieml::structure {

class NullScript: public Script {
public:
    NullScript(size_t layer) : Script(layer, "") {}

    virtual size_t get_multiplicity() const override {return 1;};
};
    
}
