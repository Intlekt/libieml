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
#include "structure/reference/ReferenceSchemaRegister.h"
#include "structure/script/ScriptRegister.h"
#include "structure/link/LinkRegister.h"
#include "SyntaxError.h"
#include "SourceMapping.h"
#include "ScriptParser.h"

#include "utils.h"


namespace ieml::parser {

class ParserContextManager {
public:
    ParserContextManager(ieml::parser::IEMLParserErrorListener* error_manager) : 
        error_manager_(error_manager), 
        script_parser_()
        {}
    
    ParserContextManager(const ParserContextManager&) = delete;
    ParserContextManager& operator=(ParserContextManager&) = delete;

    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

    structure::PathTree::Register& getPathTreeRegister() {return path_tree_register_;};
    structure::CategoryRegister& getCategoryRegister() {return category_register_;};
    structure::WordRegister& getWordRegister() {return word_register_;};
    structure::ParadigmRegister& getParadigmRegister() {return paradigm_register_;};
    structure::LinkRegister& getLinkRegister() {return link_register_;};

    structure::ReferenceSchemaRegister& getReferenceSchemaRegister() {return reference_schema_register_;};
    parser::SourceMapping& getSourceMapping() {return source_mapping_;};

    structure::ScriptRegister& getScriptRegister() {return script_register_;};

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

    structure::Script::Ptr get_or_parse_script(const std::string& s) {
        return script_parser_.get_or_parse(&script_register_, s, "", 0, 0); 
    };

    void registerLinkInstances() {
        for (const auto& link: link_register_.getLinks()) {
            const auto& range = link_register_.getFunctions(link.second);
            for (auto it = range.first; it != range.second; it++) {
                // for each function
                for (const auto& valuation : it->second.getValues(script_register_, word_register_)) {
                    const auto& schema = reference_schema_register_.get_schema(link.first);
                    const auto& ref_values = schema.reference_values_from_valuation(valuation);
                    reference_schema_register_.create_instance(link.first, ref_values);
                }
            }
        }
    }
private:
    std::shared_ptr<structure::LanguageType> default_language_;

    structure::CategoryRegister category_register_;
    structure::WordRegister word_register_;
    structure::ParadigmRegister paradigm_register_;
    structure::ReferenceSchemaRegister reference_schema_register_;
    structure::LinkRegister link_register_;

    parser::IEMLParserErrorListener* error_manager_;


    structure::PathTree::Register path_tree_register_;

    structure::ScriptRegister script_register_;

    parser::SourceMapping source_mapping_;

    ScriptParser script_parser_;
};
}