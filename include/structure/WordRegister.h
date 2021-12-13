#pragma once

#include <memory>
#include <string>

#include "structure/Word.h"
#include "structure/Namespace.h"

namespace ieml::structure {
class WordRegister {
public:
    /**********************************
     * WordRegister: Word
     **********************************/
    bool word_is_defined(std::shared_ptr<structure::Word> word) {
        return defined_words_.count(word->getScript()) > 0;
    }

    std::shared_ptr<structure::Word> get_word(const std::string& s) const {
        auto r = defined_words_.find(s);
        if (r == defined_words_.end()) return nullptr;
        return r->second;
    }

    /**********************************
     * WordRegister: Auxiliary Words
     **********************************/
    void define_auxiliary(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::AuxiliaryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_auxiliary_.define(name, word);
    }
    std::shared_ptr<structure::AuxiliaryWord> resolve_auxiliary(const structure::LanguageString& s) const {
        return namespace_auxiliary_.resolve(s);
    }
    

    /**********************************
     * WordRegister: Inflexing Words
     **********************************/
    void define_inflexing(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::InflexingWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_inflexing_.define(name, word);
    }
    std::shared_ptr<structure::InflexingWord> resolve_inflexing(const structure::LanguageString& s) const {
        return namespace_inflexing_.resolve(s);
    }
    
    /**********************************
     * WordRegister: Junction Words
     **********************************/
    void define_junction(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::JunctionWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_junction_.define(name, word);
    }
    std::shared_ptr<structure::JunctionWord> resolve_junction(const structure::LanguageString& s) const {
        return namespace_junction_.resolve(s);
    }

    /**********************************
     * WordRegister: Category Words
     **********************************/
    void define_word(std::shared_ptr<structure::CategoryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        caterory_words_.insert({word->getScript(), word});
    }
    std::shared_ptr<structure::CategoryWord> resolve_category_word(const std::string& s) const {
        auto res = caterory_words_.find(s);
        if (res == caterory_words_.end()) {
            return nullptr;
        }

        return res->second;
    }

private:
    structure::Namespace<structure::AuxiliaryWord> namespace_auxiliary_;
    structure::Namespace<structure::InflexingWord> namespace_inflexing_;
    structure::Namespace<structure::JunctionWord> namespace_junction_;

    std::unordered_map<std::string, std::shared_ptr<structure::Word>> defined_words_;
    
    std::unordered_map<std::string, std::shared_ptr<structure::CategoryWord>> caterory_words_;
};
}
