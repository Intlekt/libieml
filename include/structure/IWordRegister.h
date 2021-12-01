#pragma once

#include <memory>
#include <string>

#include "structure/Word.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class IWordRegister {
public:
    virtual bool word_is_defined(std::shared_ptr<structure::Word> word) = 0;
    virtual std::shared_ptr<structure::Word> get_word(const std::string& s) const = 0;

    /***************************
     * Auxiliaries
     ***************************/
    virtual void define_auxiliary(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::AuxiliaryWord> word) = 0;
    virtual std::shared_ptr<structure::AuxiliaryWord> resolve_auxiliary(const std::string& s) const = 0;

    /***************************
     * Inflexings
     ***************************/
    virtual void define_inflexing(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::InflexingWord> word) = 0;
    virtual std::shared_ptr<structure::InflexingWord> resolve_inflexing(const std::string& s) const = 0;
    
    /***************************
     * Junctions
     ***************************/
    virtual void define_junction(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::JunctionWord> word) = 0;
    virtual std::shared_ptr<structure::JunctionWord> resolve_junction(const std::string& s) const = 0;

    /***************************
     * Category Words
     ***************************/
    virtual void define_word(std::shared_ptr<structure::CategoryWord> word) = 0;
    virtual std::shared_ptr<structure::Word> resolve_word(const std::string& s) const = 0;

};
}
