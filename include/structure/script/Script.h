#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <memory>

#include "utils.h"


#define LAYER0_MARK ':'
#define LAYER1_MARK '.'
#define LAYER2_MARK '-'
#define LAYER3_MARK '\''
#define LAYER4_MARK ','
#define LAYER5_MARK '_'
#define LAYER6_MARK ';'

#define MAX_LAYER_NUMBER 7

#define LAYER_NUMBER(x) ieml::structure::Script::LAYER_MARK_TO_LAYER.find(x)->second


namespace ieml::structure {

class ScriptRegister;

class Script {
public:
    struct CompareScriptFunctor {
        int8_t operator()(const Script* l, const Script* r) const {
            return Script::comp(l, r) < 0;
        } 
    };

    typedef std::set<const Script*, CompareScriptFunctor> Set;

    struct HashSetFunctor {
        size_t operator()(const Set& children) const {
            size_t res = 0;
            for (auto& c : children) {
                // shared ptr must be unique (no equal script have different pointed memory)
                // ensured by the script register
                hash_combine(res, c);
            }
            return res;
        }
    };


    virtual ~Script() = default;

    const std::string& to_string() const {return str_;};
    size_t get_layer() const {return layer_;};
    const std::u16string& get_canonical() const {return canonical_;};
    // const Set& get_singular_sequences() const {return singular_sequences_;}
    size_t get_multiplicity() const {return multiplicity_;};
    
    const Set& singular_sequences() const {
        return singular_sequences_;
    };


    // by default not a null script
    virtual bool is_nullscript() const {return false;};

    static const char LAYER_MARKS[MAX_LAYER_NUMBER];
    static const std::unordered_map<char, size_t> LAYER_MARK_TO_LAYER;

protected:
    Set singular_sequences_;

    Script(size_t layer, 
           const std::string& str, 
           const std::u16string& canonical, 
           size_t multiplicity) :
        singular_sequences_(),
        layer_(layer), 
        str_(str), 
        canonical_(canonical),
        multiplicity_(multiplicity) {}
private:


    const size_t layer_;
    const std::string str_;
    const std::u16string canonical_;

    const size_t multiplicity_;

    static size_t _count_multiplicity(const Script::Set& singular_sequences) {
        if (singular_sequences.size() == 0) return 1;
        return singular_sequences.size();
    }

    static int comp(const Script*, const Script*);
};
}
