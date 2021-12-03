#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "structure/Path.h"
#include "structure/Word.h"
#include "structure/LanguageString.h"
#include "structure/WordRegister.h"
#include "structure/CategoryRegister.h"
#include "RecognitionError.h"

#include "utils.h"


namespace ieml::parser {
class ParserContext : public ieml::structure::WordRegister {
public:
    ParserContext(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager), 
        default_language_(std::make_shared<structure::LanguageType>(structure::LanguageType::FR)) {}

    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};
    structure::PathTree::Register& getPathTreeRegister() {return path_tree_register_;};
    
    structure::CategoryRegister& getCategoryRegister() {return category_register_;};

    std::shared_ptr<structure::LanguageType> getDefaultLanguage() const {return default_language_;};

    /**********************************
     * WordRegister: Word
     **********************************/

    virtual bool word_is_defined(std::shared_ptr<structure::Word> word) {
        return defined_words_.count(word->getScript()) > 0;
    }

    virtual std::shared_ptr<structure::Word> get_word(const std::string& s) const {
        auto r = defined_words_.find(s);
        if (r == defined_words_.end()) return nullptr;
        return r->second;
    }

    /**********************************
     * WordRegister: Auxiliary Words
     **********************************/
    virtual void define_auxiliary(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::AuxiliaryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_auxiliary_.define(name, word);
    }
    virtual std::shared_ptr<structure::AuxiliaryWord> resolve_auxiliary(const std::string& s) const {
        return namespace_auxiliary_.resolve(structure::LanguageString(*default_language_, s));
    }
    

    /**********************************
     * WordRegister: Inflexing Words
     **********************************/
    virtual void define_inflexing(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::InflexingWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_inflexing_.define(name, word);
    }
    virtual std::shared_ptr<structure::InflexingWord> resolve_inflexing(const std::string& s) const {
        return namespace_inflexing_.resolve(structure::LanguageString(*default_language_, s));
    }
    
    /**********************************
     * WordRegister: Junction Words
     **********************************/
    virtual void define_junction(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::JunctionWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        namespace_junction_.define(name, word);
    }
    virtual std::shared_ptr<structure::JunctionWord> resolve_junction(const std::string& s) const {
        return namespace_junction_.resolve(structure::LanguageString(*default_language_, s));
    }

    /**********************************
     * WordRegister: Category Words
     **********************************/
    virtual void define_word(std::shared_ptr<structure::CategoryWord> word) {
        if (defined_words_.count(word->getScript()) > 0)
            throw std::invalid_argument("Word already defined.");

        defined_words_.insert({word->getScript(), word});
        caterory_words_.insert({word->getScript(), word});
    }
    virtual std::shared_ptr<structure::CategoryWord> resolve_category_word(const std::string& s) const {
        auto res = caterory_words_.find(s);
        if (res == caterory_words_.end()) {
            return nullptr;
        }

        return res->second;
    }


private:
    std::shared_ptr<structure::LanguageType> default_language_;

    std::unordered_map<std::string, std::shared_ptr<structure::Word>> defined_words_;

    structure::Namespace<structure::AuxiliaryWord> namespace_auxiliary_;
    structure::Namespace<structure::InflexingWord> namespace_inflexing_;
    structure::Namespace<structure::JunctionWord> namespace_junction_;
    
    std::unordered_map<std::string, std::shared_ptr<structure::CategoryWord>> caterory_words_;
    
    structure::CategoryRegister category_register_;

    parser::IEMLParserErrorListener* error_manager_;

    structure::PathTree::Register path_tree_register_;
};
}