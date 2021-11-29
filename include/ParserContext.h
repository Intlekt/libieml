#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "structure/Path.h"
#include "structure/Phrase.h"
#include "structure/Word.h"
#include "structure/LanguageString.h"
#include "RecognitionError.h"

#include "utils.h"


namespace ieml::parser {
class ParserContext {
public:
    ParserContext(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager), 
        default_language_(structure::LanguageType::FR) {}

    // ieml::structure::Namespace& getNamespace() const {return *namespace_;};
    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

    void define_auxiliary(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::AuxiliaryWord> word) {
        namespace_auxiliary_.define(name, word);
    }
    std::shared_ptr<structure::AuxiliaryWord> resolve_auxiliary(const std::string& s) const {
        return namespace_auxiliary_.resolve(structure::LanguageString(default_language_, s));
    }

    void define_inflexion(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::InflexingWord> word) {
        namespace_inflexing_.define(name, word);
    }
    std::shared_ptr<structure::InflexingWord> resolve_inflexion(const std::string& s) const {
        return namespace_inflexing_.resolve(structure::LanguageString(default_language_, s));
    }

    void define_word(std::shared_ptr<structure::Word> word) {
        defined_words_.insert({word->getScript(), word});
    }
    std::shared_ptr<structure::Word> resolve_word(const std::string& s) const {
        auto res = defined_words_.find(s);
        if (res == defined_words_.end()) {
            return nullptr;
        }

        return res->second;
    }

    void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::Phrase> phrase, bool is_node) {
        namespace_category_.define(name, phrase);
        category_is_node_.insert({phrase, is_node});
    }
    std::shared_ptr<structure::Phrase> resolve_category(const std::string& s) const {
        return namespace_category_.resolve(structure::LanguageString(default_language_, s));
    }




private:
    structure::LanguageType default_language_;

    structure::Namespace<structure::AuxiliaryWord> namespace_auxiliary_;
    structure::Namespace<structure::InflexingWord> namespace_inflexing_;
    structure::Namespace<structure::JunctionWord> namespace_junction_;

    std::unordered_map<std::string, std::shared_ptr<structure::Word>> defined_words_;
    
    structure::Namespace<structure::Phrase> namespace_category_;

    std::unordered_map<std::shared_ptr<structure::Phrase>, bool> category_is_node_;

    parser::IEMLParserErrorListener* error_manager_;
};
}