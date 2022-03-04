#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <sstream>

#include "structure/script/Script.h"


#define REMARKABLE_MULTIPLICATION(c) ieml::structure::MultiplicativeScript::REMARKABLE_MULTIPLICATIONS.find(c)->second


namespace ieml::structure {

class MultiplicativeScript : public Script {
public:
    MultiplicativeScript(
        std::shared_ptr<Script> substance,
        std::shared_ptr<Script> attribute,
        std::shared_ptr<Script> mode     
    ) : 
        Script(substance->get_layer() + 1, string_repr(substance, attribute, mode)), 
        substance_(std::move(substance)), 
        attribute_(std::move(attribute)),
        mode_(std::move(mode)) {}

    virtual size_t get_multiplicity() const override {return 1;};

    static const std::unordered_map<std::string, std::shared_ptr<MultiplicativeScript>> REMARKABLE_MULTIPLICATIONS;

private:
    const std::shared_ptr<Script> substance_;
    const std::shared_ptr<Script> attribute_;
    const std::shared_ptr<Script> mode_;


    static std::string string_repr(std::shared_ptr<Script> substance, std::shared_ptr<Script> attribute, std::shared_ptr<Script> mode) {
        std::stringstream os;

        os << substance->to_string();
        if (attribute) os << attribute->to_string();
        if (mode) os << mode->to_string();

        os << LAYER_MARKS[substance->get_layer() + 1];
        return os.str();
    };

};

}