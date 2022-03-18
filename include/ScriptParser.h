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
    /*
    */
    ScriptParser();

    const ieml::structure::Script* get_or_parse(
        ieml::structure::ScriptRegister* reg,
        const std::string& input_str, 
        const std::string& file_id, 
        const size_t line_offset, 
        const size_t char_offset);

    ScriptParser(const ScriptParser&) = delete;
    ScriptParser& operator=(ScriptParser&) = delete;

private:

    // BASIC ANTLR ERROR LISTENER TO TRACK PARSE ERROR
    class ANTLR4ScriptParserErrorListener: public ANTLRErrorListener {
    public:
        ANTLR4ScriptParserErrorListener() : has_error_(false) {}

        ANTLR4ScriptParserErrorListener(const ANTLR4ScriptParserErrorListener&) = delete;
        ANTLR4ScriptParserErrorListener& operator=(ANTLR4ScriptParserErrorListener&) = delete;

        void syntaxError(Recognizer *, Token *, size_t , size_t , const std::string &, std::exception_ptr ) {
            has_error_ = true;
        }

        void reportAmbiguity(Parser *, const dfa::DFA &, size_t , size_t , bool ,
                            const antlrcpp::BitSet &, atn::ATNConfigSet *) {}

        void reportAttemptingFullContext(Parser *, const dfa::DFA &, size_t , size_t ,
                                        const antlrcpp::BitSet &, atn::ATNConfigSet *){}

        void reportContextSensitivity(Parser *, const dfa::DFA &, size_t , size_t ,
                                    size_t , atn::ATNConfigSet *) {}

        void clear() {has_error_ = false;}
        bool has_error() const {return has_error_;}
    private:
        bool has_error_;
    };

    std::unique_ptr<antlr4::ANTLRInputStream> input_ = nullptr;
    std::unique_ptr<antlr4::CommonTokenStream> tokens_ = nullptr;

    std::unique_ptr<script_generated::ScriptLexerGrammar> lexer_ = nullptr;
    std::unique_ptr<script_generated::ScriptParserGrammar> parser_ = nullptr;

    std::shared_ptr<ScriptGrammarVisitor> visitor_;

    ANTLR4ScriptParserErrorListener antlr_error_listener_;
};

}