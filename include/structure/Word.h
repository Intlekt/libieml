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

    virtual WordType getWordType() const = 0;

    // total ordering on Word and subtypes
    bool operator==(const Word& word) const {return getWordType() == word.getWordType() && script_ == word.script_;}
    bool operator!=(const Word& word) const {return getWordType() != word.getWordType() || script_ != word.script_;}
    bool operator< (const Word& word) const {return getWordType() <  word.getWordType() || script_ <  word.script_;}
    bool operator> (const Word& word) const {return getWordType() >  word.getWordType() || script_ >  word.script_;}
    bool operator<=(const Word& word) const {return getWordType() <= word.getWordType() && script_ <= word.script_;}
    bool operator>=(const Word& word) const {return getWordType() >= word.getWordType() && script_ >= word.script_;}

private:
    const std::string script_;

};

class CategoryWord: public Word {
public:
    CategoryWord(const std::string& s): Word(s) {}
    virtual WordType getWordType() const {return WordType::CATEGORY;};
};

class AuxiliaryWord: public Word {
public:
    AuxiliaryWord(const std::string& s, RoleType accepted_role) : 
        Word(s), accepted_role_(accepted_role) {}

    bool accept_role(RoleType role_type) const {
        return accepted_role_ == role_type;
    }
    virtual WordType getWordType() const {return WordType::AUXILIARY;};

private:
    const RoleType accepted_role_;
};

class InflexingWord: public Word {
public:
    InflexingWord(const std::string& s, InflexingType type) : 
        Word(s), type_(type) {}

    /**
     * NOUN inflexion can be anywhere, but VERB inflexions can only be in ROOT. 
     */
    bool accept_role(RoleType role_type) {
        return type_ == +InflexingType::NOUN || role_type == +RoleType::ROOT;
    }
    virtual WordType getWordType() const {return WordType::INFLECTION;};

private:
    const InflexingType type_;
};

class JunctionWord: public Word {
public:
    JunctionWord(const std::string& s) : 
        Word(s) {}

    virtual WordType getWordType() const {return WordType::JUNCTION;};
};
}

// define hash
namespace std {
template<>
struct hash<ieml::structure::CategoryWord> {
    size_t operator()(const ieml::structure::CategoryWord& s) const noexcept {
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
struct hash<ieml::structure::JunctionWord> {
    size_t operator()(const ieml::structure::JunctionWord& s) const noexcept {
        return hash<std::string>{}(s.getScript());
    }
};
}

STD_HASH_SHARED_PTR(ieml::structure::CategoryWord);
STD_HASH_SHARED_PTR(ieml::structure::AuxiliaryWord);
STD_HASH_SHARED_PTR(ieml::structure::InflexingWord);
STD_HASH_SHARED_PTR(ieml::structure::JunctionWord);

STD_LESS_SHARED_PTR(ieml::structure::CategoryWord);
STD_LESS_SHARED_PTR(ieml::structure::AuxiliaryWord);
STD_LESS_SHARED_PTR(ieml::structure::InflexingWord);
STD_LESS_SHARED_PTR(ieml::structure::JunctionWord);

STD_EQUAL_TO_SHARED_PTR(ieml::structure::CategoryWord);
STD_EQUAL_TO_SHARED_PTR(ieml::structure::AuxiliaryWord);
STD_EQUAL_TO_SHARED_PTR(ieml::structure::InflexingWord);
STD_EQUAL_TO_SHARED_PTR(ieml::structure::JunctionWord);
