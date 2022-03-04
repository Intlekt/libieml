#pragma once

#include <memory>
#include <vector>
#include <sstream>

#include "structure/script/Script.h"
#include "structure/script/PrimitiveScript.h"


#define REMARKABLE_ADDITION(c) ieml::structure::AdditiveScript::REMARKABLE_ADDITIONS.find(c)->second


namespace ieml::structure {

class AdditiveScript : public Script {
public:
    AdditiveScript(std::vector<std::shared_ptr<Script>>&& children) : 
        Script(children[0]->get_layer(), string_repr(children)), 
        children_(std::move(children)) {}

    static const std::unordered_map<char, std::shared_ptr<AdditiveScript>> REMARKABLE_ADDITIONS;
private:
    const std::vector<std::shared_ptr<Script>> children_;


    static std::string string_repr(std::vector<std::shared_ptr<Script>> children) {
        std::stringstream os;

        bool first = true;
        for (auto& c : children) {
            if (first) first = false;
            else       os << "+";
            os << c->to_string();
        }
        return os.str();
    };

};

}