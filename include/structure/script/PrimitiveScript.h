#pragma once

#include "structure/script/Script.h"


#include <unordered_map>
#include <memory>


#define PRIMITIVE(c) ieml::structure::PrimitiveScript::PRIMITIVES.find(c)->second

namespace ieml::structure {

class PrimitiveScript: public Script {
public:
    virtual size_t get_multiplicity() const override {return 1;};

    // all the primitives
    static const std::unordered_map<char, std::shared_ptr<PrimitiveScript>> PRIMITIVES;

private:
    PrimitiveScript(char c) : Script(0, std::string(1, char_) + LAYER0_MARK), char_(c) {}

    const char char_;
};


}