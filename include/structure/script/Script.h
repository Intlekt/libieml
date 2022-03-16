#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <memory>
#include <functional>

#include "structure/table/Table.h"
#include "utils.h"

#include "structure/Element.h"

#include "enum.h"

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

BETTER_ENUM(ScriptType, char, PRIMITIVE, NULLSCRIPT, ADDITION, MULTIPLICATION)

class ScriptRegister;

class Script : public Element {
public:
    friend class ScriptRegister;

    typedef const Script* Ptr;
    typedef const Table<const Script*>* TablePtr;

    struct CompareScriptFunctor {
        int8_t operator()(const Script* l, const Script* r) const {
            return l->comp_element_(*r) < 0;
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

    virtual std::string to_string() const override {return str_;};
    virtual ElementType getElementType() const override {return ElementType::SCRIPT;};
    virtual std::string prefix() const override {return "script";};
    virtual size_t hash() const override {return std::hash<std::string>{}(str_);};

    size_t get_layer() const {return layer_;};
    size_t get_multiplicity() const {return multiplicity_;};

    const std::u16string& get_canonical() const {return canonical_;};
    const Set& singular_sequences() const {return singular_sequences_;};

    virtual ScriptType get_type() const = 0;

    // by default not a null script
    virtual bool is_nullscript() const {return false;};

    static const char LAYER_MARKS[MAX_LAYER_NUMBER];
    static const std::unordered_map<char, size_t> LAYER_MARK_TO_LAYER;

protected:
    virtual TablePtr build_table_paradigm(ScriptRegister&) const = 0;

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

    virtual int comp_element_(const Element&) const override;

    /**
     * @brief Allocate a table pointer representing this script and return it. The caller own the result.
     * 
     * @return TablePtr
     */
    TablePtr get_table(ScriptRegister&) const;
};
}
