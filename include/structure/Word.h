#pragma once

#include <string>
#include <set>
#include <unordered_set>

#include "structure/Constants.h"
#include "utils.h"

#include "structure/Element.h"

namespace ieml::structure {
class Word : public Element {
public:
    Word(const std::string& s): script_(s) {}

    typedef std::shared_ptr<Word> Ptr;

    virtual ElementType getElementType() const {return ElementType::WORD;};

    virtual std::string to_string() const { return "\"" + script_ + "\""; };

    std::string getScript() const {return script_;};

    virtual WordType getWordType() const = 0;

    virtual size_t hash() const {
        auto hasher = std::hash<std::string>();
        return hasher(script_);
    };


protected:
    virtual int comp_element_(const Element& o_word) const {
        const auto& o = dynamic_cast<const Word&>(o_word);

        if (getWordType() == o.getWordType()) {
            if (script_ == o.script_)               return 0;
            else if (script_ < o.script_)           return -1;
            else                                    return 1; 
        } else if (getWordType() < o.getWordType()) return -1;
        else                                        return 1;
    };


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

    RoleType getRoleType() const {return accepted_role_;}

private:
    const RoleType accepted_role_;
};

class InflectionWord: public Word {
public:
    InflectionWord(const std::string& s, InflectionType type) : 
        Word(s), type_(type) {}

    /**
     * NOUN inflection can be anywhere, but VERB inflections can only be in ROOT. 
     */
    bool accept_role(RoleType role_type) const {
        return (type_ == +InflectionType::NOUN || role_type == +RoleType::ROOT);
    }
    virtual WordType getWordType() const override {return WordType::INFLECTION;};

private:
    const InflectionType type_;
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
struct hash<ieml::structure::InflectionWord> {
    size_t operator()(const ieml::structure::InflectionWord& s) const noexcept {
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

STD_HASH_SHARED_PTR(ieml::structure::CategoryWord)
STD_HASH_SHARED_PTR(ieml::structure::AuxiliaryWord)
STD_HASH_SHARED_PTR(ieml::structure::InflectionWord)
STD_HASH_SHARED_PTR(ieml::structure::JunctionWord)

STD_LESS_SHARED_PTR(ieml::structure::CategoryWord)
STD_LESS_SHARED_PTR(ieml::structure::AuxiliaryWord)
STD_LESS_SHARED_PTR(ieml::structure::InflectionWord)
STD_LESS_SHARED_PTR(ieml::structure::JunctionWord)

STD_EQUAL_TO_SHARED_PTR(ieml::structure::CategoryWord)
STD_EQUAL_TO_SHARED_PTR(ieml::structure::AuxiliaryWord)
STD_EQUAL_TO_SHARED_PTR(ieml::structure::InflectionWord)
STD_EQUAL_TO_SHARED_PTR(ieml::structure::JunctionWord)
