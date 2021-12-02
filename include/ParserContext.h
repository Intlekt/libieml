#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "structure/Path.h"
#include "structure/Word.h"
#include "structure/LanguageString.h"
#include "structure/IWordRegister.h"
#include "RecognitionError.h"

#include "utils.h"


namespace ieml::parser {
class ParserContext : public ieml::structure::IWordRegister {
public:
    ParserContext(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager), 
        default_language_(structure::LanguageType::FR) {}

    // ieml::structure::Namespace& getNamespace() const {return *namespace_;};
    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

    virtual bool word_is_defined(std::shared_ptr<structure::Word> word) {
        return defined_words_.count(word->getScript()) > 0;
    }

    virtual std::shared_ptr<structure::Word> get_word(const std::string& s) const {
        auto r = defined_words_.find(s);
        if (r == defined_words_.end()) return nullptr;
        return r->second;
    }


    /***************************
     * Auxiliaries
     ***************************/
    virtual void define_auxiliary(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::AuxiliaryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_auxiliary_.define(name, word);
    }
    virtual std::shared_ptr<structure::AuxiliaryWord> resolve_auxiliary(const std::string& s) const {
        return namespace_auxiliary_.resolve(structure::LanguageString(default_language_, s));
    }
    

    /***************************
     * Inflexings
     ***************************/
    virtual void define_inflexing(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::InflexingWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_inflexing_.define(name, word);
    }
    virtual std::shared_ptr<structure::InflexingWord> resolve_inflexing(const std::string& s) const {
        return namespace_inflexing_.resolve(structure::LanguageString(default_language_, s));
    }
    
    /***************************
     * Junctions
     ***************************/
    virtual void define_junction(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::JunctionWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_junction_.define(name, word);
    }
    virtual std::shared_ptr<structure::JunctionWord> resolve_junction(const std::string& s) const {
        return namespace_junction_.resolve(structure::LanguageString(default_language_, s));
    }

    /***************************
     * Category Words
     ***************************/
    virtual void define_word(std::shared_ptr<structure::CategoryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        caterory_words_.insert({word->getScript(), word});
    }
    virtual std::shared_ptr<structure::Word> resolve_word(const std::string& s) const {
        auto res = caterory_words_.find(s);
        if (res == caterory_words_.end()) {
            return nullptr;
        }

        return res->second;
    }

    /***************************
     * Categories
     ***************************/
    virtual void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::PathTree> phrase, bool is_node) {
        if (category_is_node_.count(phrase) > 0) 
            throw std::invalid_argument("Phrase already defined.");
        
        category_is_node_.insert({phrase, is_node});
        namespace_category_.define(name, phrase);
    }
    virtual std::shared_ptr<structure::PathTree> resolve_category(const std::string& s) const {
        return namespace_category_.resolve(structure::LanguageString(default_language_, s));
    }

    structure::PathTree::Register& getPathTreeRegister() {return path_tree_register_;};

private:
    structure::LanguageType default_language_;

    std::unordered_map<std::string, std::shared_ptr<structure::Word>> defined_words_;

    structure::Namespace<structure::AuxiliaryWord> namespace_auxiliary_;
    structure::Namespace<structure::InflexingWord> namespace_inflexing_;
    structure::Namespace<structure::JunctionWord> namespace_junction_;
    
    std::unordered_map<std::string, std::shared_ptr<structure::CategoryWord>> caterory_words_;
    
    structure::Namespace<structure::PathTree> namespace_category_;
    std::unordered_map<std::shared_ptr<structure::PathTree>, bool> category_is_node_;

    parser::IEMLParserErrorListener* error_manager_;

    structure::PathTree::Register path_tree_register_;
};
}