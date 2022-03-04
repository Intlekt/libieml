#pragma once


#include <string>

#define LAYER0_MARK ':'
#define LAYER1_MARK '.'
#define LAYER2_MARK '-'
#define LAYER3_MARK '\''
#define LAYER4_MARK ','
#define LAYER5_MARK '_'
#define LAYER6_MARK ';'


namespace ieml::structure {
class Script {
public:
    Script(size_t layer, const std::string& str) : layer_(layer), str_(str) {}

    std::string to_string() const {return str_;};
    size_t get_layer() const {return layer_;};

    virtual size_t get_multiplicity() const = 0;

    static const char LAYER_MARKS[7];
private:
    const size_t layer_;
    const std::string str_;
};
}
