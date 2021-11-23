#pragma once

#include <string>


namespace ieml::structure {
class Word {
public:
    Word(const std::string& s): script_(s) {}

    std::string to_string() const {
        return script_;
    }

private:
    const std::string script_;

};


}