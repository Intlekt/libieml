#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "structure/path/PathTree.h"
#include "structure/Word.h"
#include "structure/LanguageString.h"
#include "structure/WordRegister.h"
#include "structure/CategoryRegister.h"
#include "structure/ParadigmRegister.h"
#include "SyntaxError.h"
#include "SourceMapping.h"

#include "utils.h"


namespace ieml::parser {

class ParserContextManager {
public:
    ParserContextManager(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager) {}

    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};
    structure::PathTree::Register& getPathTreeRegister() {return path_tree_register_;};
    structure::CategoryRegister& getCategoryRegister() {return category_register_;};
    structure::WordRegister& getWordRegister() {return word_register_;};
    structure::ParadigmRegister& getParadigmRegister() {return paradigm_register_;};
    parser::SourceMapping& getSourceMapping() {return source_mapping_;};


    structure::LanguageType getLanguage() const {
        if (default_language_ == nullptr)
            return structure::LanguageType::EN;
        else 
            return *default_language_;
    };
    
    void resetLanguage() {
        default_language_ = nullptr;
    }

    bool setLanguage(structure::LanguageType language_type) {
        if (default_language_ != nullptr) {
            return false;
        }
        default_language_ = std::make_shared<structure::LanguageType>(language_type);
        return true;
    };

private:
    std::shared_ptr<structure::LanguageType> default_language_;

    structure::CategoryRegister category_register_;
    structure::WordRegister word_register_;
    structure::ParadigmRegister paradigm_register_;

    parser::IEMLParserErrorListener* error_manager_;

    structure::PathTree::Register path_tree_register_;

    parser::SourceMapping source_mapping_;
};
}