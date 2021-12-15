#include "SyntaxError.h"
#include "Token.h"


#include <iostream>


using namespace ieml::parser;

void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::syntaxError(
                Recognizer *recognizer, 
                Token *offendingSymbol, size_t line,
                size_t charPositionInLine, 
                const std::string &msg, 
                std::exception_ptr e) {
    
    size_t char_len;
    if (offendingSymbol != nullptr)
        char_len = offendingSymbol->getText().size();
    else
        char_len = 1;

    error_manager_->registerError(new ParseError(
        ieml::AST::CharRange(
            file_id_,
            line, 
            line, 
            charPositionInLine, 
            charPositionInLine + char_len
        ), msg));
};
void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact,
                                              const antlrcpp::BitSet &ambigAlts, atn::ATNConfigSet *configs) {


};
void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                                          const antlrcpp::BitSet &conflictingAlts, atn::ATNConfigSet *configs) {

};
void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                                       size_t prediction, atn::ATNConfigSet *configs) {

};

void IEMLParserErrorListener::visitorError(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerError(new VisitorError(char_range, msg));
};

void IEMLParserErrorListener::parseError(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerError(new ParseError(char_range, msg));
};

