#pragma once

#include <string>
#include <set>
#include <unordered_set>

#include "structure/Constants.h"
#include "utils.h"


namespace ieml::structure {
class Word {
public:
    Word(const std::string& s): script_(s) {}

    std::string to_string() const { return "'" + script_ + "'"; };

    std::string getScript() const {return script_;};

    bool operator==(const Word& word) const {
        return script_ == word.script_;
    }

private:
    const std::string script_;

};

class AuxiliaryWord: public Word {
public:
    AuxiliaryWord(const std::string& s, std::unordered_set<RoleType> accepted_roles) : 
        Word(s), accepted_roles_(accepted_roles) {}

    bool accept_role(RoleType role_type) const {
        return accepted_roles_.count(role_type) > 0;
    }

private:
    const std::unordered_set<RoleType> accepted_roles_;
};

class InflexingWord: public Word {
public:
    InflexingWord(const std::string& s) : 
        Word(s) {}
};


}

namespace std {

template<>
struct hash<ieml::structure::Word> {
    size_t operator()(const ieml::structure::Word& s) const noexcept {
        return hash<std::string>{}(s.getScript());
    }
};

template<>
struct hash<ieml::structure::AuxiliaryWord> {
    size_t operator()(const ieml::structure::AuxiliaryWord& s) const noexcept {
        return hash<std::string>{}(s.getScript());
    }
};

template<>
struct hash<ieml::structure::InflexingWord> {
    size_t operator()(const ieml::structure::InflexingWord& s) const noexcept {
        return hash<std::string>{}(s.getScript());
    }
};

template<>
struct less<ieml::structure::Word> {
    size_t operator()(const ieml::structure::Word& l, const ieml::structure::Word& r) const noexcept {
        return less<std::string>{}(l.getScript(), r.getScript());
    }
};

template<>
struct less<ieml::structure::AuxiliaryWord> {
    size_t operator()(const ieml::structure::AuxiliaryWord& l, const ieml::structure::AuxiliaryWord& r) const noexcept {
        return less<std::string>{}(l.getScript(), r.getScript());
    }
};

template<>
struct less<ieml::structure::InflexingWord> {
    size_t operator()(const ieml::structure::InflexingWord& l, const ieml::structure::InflexingWord& r) const noexcept {
        return less<std::string>{}(l.getScript(), r.getScript());
    }
};
STD_HASH_SHARED_PTR(ieml::structure::Word);
STD_HASH_SHARED_PTR(ieml::structure::AuxiliaryWord);
STD_HASH_SHARED_PTR(ieml::structure::InflexingWord);

STD_LESS_SHARED_PTR(ieml::structure::Word);
STD_LESS_SHARED_PTR(ieml::structure::AuxiliaryWord);
STD_LESS_SHARED_PTR(ieml::structure::InflexingWord);

}