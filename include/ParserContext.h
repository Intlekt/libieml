#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "structure/Path.h"
#include "structure/Phrase.h"
#include "structure/LanguageString.h"
#include "RecognitionError.h"


namespace ieml::parser {
class ParserContext {
public:
    ParserContext(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager), 
        default_language_(structure::LanguageType::FR), 
        namespace_(std::make_unique<structure::Namespace>()) {}

    // ieml::structure::Namespace& getNamespace() const {return *namespace_;};
    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

    structure::AuxiliaryType resolve_auxiliary(const std::string& s) const {
        return structure::AuxiliaryType::ABOVE;
    }

    structure::InflexingType resolve_inflexion(const std::string& s) const {
        return structure::InflexingType::PLURAL;
    }

    void define_category(std::shared_ptr<structure::Name> name, std::shared_ptr<structure::Phrase> phrase, bool is_node) {
        namespace_->define(name, phrase, is_node);
    }

    std::shared_ptr<structure::Phrase> resolve_category(const std::string& s) const {
        return namespace_->resolve(structure::LanguageString(default_language_, s));
    }

    std::shared_ptr<structure::WordPathNode> resolve_word(const std::string& s) const {
        return std::make_shared<structure::WordPathNode>(structure::Word(s));
    }


private:
    structure::LanguageType default_language_;
    std::unique_ptr<structure::Namespace> namespace_;
    parser::IEMLParserErrorListener* error_manager_;
};
}