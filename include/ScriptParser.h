#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "antlr4-runtime.h"
#include "ScriptLexerGrammar.h"
#include "ScriptParserGrammar.h"

#include "SyntaxError.h"

#include "ScriptGrammarVisitor.h"

#include "structure/script/Script.h"
#include "structure/script/ScriptRegister.h"


namespace ieml::parser {

class ScriptParser {
public:
    ScriptParser(const std::string& file_id, IEMLParserErrorListener* error_manager);

    const ieml::structure::Script* get_or_parse(
        ieml::structure::ScriptRegister* reg,
        const std::string& input_str, 
        const std::string& file_id, 
        const size_t line_offset, 
        const size_t char_offset);

    ScriptParser(const ScriptParser&) = delete;

private:
    std::unique_ptr<antlr4::ANTLRInputStream> input_ = nullptr;
    std::unique_ptr<antlr4::CommonTokenStream> tokens_ = nullptr;

    std::unique_ptr<script_generated::ScriptLexerGrammar> lexer_ = nullptr;
    std::unique_ptr<script_generated::ScriptParserGrammar> parser_ = nullptr;

    std::shared_ptr<ScriptGrammarVisitor> visitor_;
};

}