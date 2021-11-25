#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "structure/Constants.h"
#include "RecognitionError.h"


namespace ieml::parser {
class ParserContext {
public:
    ParserContext(ieml::parser::IEMLParserErrorListener* error_manager) : error_manager_(error_manager) {}

    ieml::structure::Namespace& getNamespace() const {return *namespace_;};
    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

    structure::AuxiliaryType resolve_auxiliary(const std::string& s) const {
        return structure::AuxiliaryType::ABOVE;
    }

private:
    std::unique_ptr<ieml::structure::Namespace> namespace_;
    ieml::parser::IEMLParserErrorListener* error_manager_;
};
}