#pragma once

#include <memory>

#include "structure/Namespace.h"
#include "RecognitionError.h"


namespace ieml::parser {
class ParserContext {
public:
    ieml::structure::Namespace& getNamespace() const {return *namespace_;};
    ieml::parser::IEMLParserErrorListener& getErrorManager() const {return *error_manager_;};

private:
    std::unique_ptr<ieml::structure::Namespace> namespace_;
    std::shared_ptr<ieml::parser::IEMLParserErrorListener> error_manager_;
};


}