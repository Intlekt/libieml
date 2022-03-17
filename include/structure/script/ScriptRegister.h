#pragma once

#include <unordered_map>
#include <array>

#include "structure/script/Script.h"
#include "structure/script/MultiplicativeScript.h"
#include "structure/script/AdditiveScript.h"
#include "structure/table/Table.h"


namespace ieml::structure {

class ScriptRegister {
public:
    ScriptRegister() : 
        primitives_(),
        null_scripts_(),
        multiplicative_scripts_(), 
        remarkable_multiplications_(),
        additive_scripts_(),
        remarkable_additions_() {
            _build_primitives();
            _build_null_scripts();
            _build_remarkable_additions();
            _build_remarkable_multiplications();
        }
    ~ScriptRegister() {
        for (const auto& add: additive_scripts_)
            delete add.second;
        
        for (const auto& mul: multiplicative_scripts_)
            delete mul.second;

        for (const auto& p: primitives_)
            delete p.second;

        for (const auto& p: null_scripts_)
            delete p;

        for (const auto& table: tables_)
            delete table.second;
    }

    ScriptRegister(const ScriptRegister&) = delete;
    ScriptRegister& operator=(ScriptRegister&) = delete;

    const MultiplicativeScript* get_or_create_multiplication(const MultiplicativeScript::Children& children);
    const AdditiveScript* get_or_create_addition(const Script::Set& children);
    Script::TablePtr get_or_create_table(const Script* script);


    const PrimitiveScript* get_primitive(char c) const {return primitives_.find(c)->second;};
    const NullScript* get_nullscript(size_t layer) const {return null_scripts_[layer];};

    const MultiplicativeScript* promote(const Script* substance) {
        const auto nullscript = get_nullscript(substance->get_layer());
        return get_or_create_multiplication({substance, nullscript, nullscript});
    }

    const MultiplicativeScript* get_remarkable_multiplication(const std::string& tag) const {
        return remarkable_multiplications_.find(tag)->second;
    }
    const AdditiveScript* get_remarkable_addition(char tag) const {
        return remarkable_additions_.find(tag)->second;
    }

    Script::Ptr get_defined_script_by_string(const std::string& s) const {
        auto it = defined_scripts_by_string_.find(s);
        if (it == defined_scripts_by_string_.end()) return nullptr;
        return it->second;
    }

    const std::unordered_map<std::string, Script::Ptr>& defined_script() const {return defined_scripts_by_string_;};

    const std::unordered_map<Script::Ptr, Script::TablePtr>& get_tables() const {return tables_;};
private:
    std::unordered_map<std::string, Script::Ptr> defined_scripts_by_string_;

    // Primitives
    std::unordered_map<char, const PrimitiveScript*> primitives_;

    // Null scripts
    std::array<const NullScript*, MAX_LAYER_NUMBER> null_scripts_;

    // Multiplications
    std::unordered_map<MultiplicativeScript::Children, const MultiplicativeScript*, MultiplicativeScript::HashChildrenFunctor> multiplicative_scripts_;
    std::unordered_map<std::string, const MultiplicativeScript*> remarkable_multiplications_;

    // Additions
    std::unordered_map<Script::Set, const AdditiveScript*, Script::HashSetFunctor> additive_scripts_;
    std::unordered_map<char, const AdditiveScript*> remarkable_additions_;

    // Tables
    // Store for each script its table
    std::unordered_map<Script::Ptr, Script::TablePtr> tables_;


    void define_script(Script::Ptr);

    void _build_primitives();
    void _build_null_scripts();
    void _build_remarkable_additions();
    void _build_remarkable_multiplications();
};

}