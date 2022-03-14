#include "SyntaxError.h"
#include "Token.h"

#include "Parser.h"

#include <iostream>


using namespace ieml::parser;

void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::syntaxError(
                __attribute__ ((unused)) Recognizer *recognizer, 
                Token *offendingSymbol, size_t line,
                size_t charPositionInLine, 
                const std::string &msg, 
                __attribute__ ((unused)) std::exception_ptr e) {
    
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
}

void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportAmbiguity(Parser *recognizer, 
                                                                            __attribute__ ((unused)) const dfa::DFA &dfa, 
                                                                            size_t startIndex, 
                                                                            size_t stopIndex, 
                                                                             __attribute__ ((unused)) bool exact,
                                                                             __attribute__ ((unused)) const antlrcpp::BitSet &ambigAlts, 
                                                                             __attribute__ ((unused)) atn::ATNConfigSet *configs) {
    const auto text = recognizer->getTokenStream()->getText(antlr4::misc::Interval(startIndex, stopIndex));
    std::cout << "Parser ambiguity detected :" << text<< std::endl;

}
void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportAttemptingFullContext( Parser *recognizer, 
                                                                                         __attribute__ ((unused)) const dfa::DFA &dfa, 
                                                                                         size_t startIndex, 
                                                                                         size_t stopIndex, 
                                                                                         __attribute__ ((unused)) const antlrcpp::BitSet &conflictingAlts, 
                                                                                         __attribute__ ((unused)) atn::ATNConfigSet *configs) {
    const auto text = recognizer->getTokenStream()->getText(antlr4::misc::Interval(startIndex, stopIndex));
    std::cout << "Parser reportAttemptingFullContext detected : " << text << std::endl;

}
void IEMLParserErrorListener::ANTLR4IEMLParserErrorListener::reportContextSensitivity(Parser *recognizer, 
                                                                                         __attribute__ ((unused)) const dfa::DFA &dfa, 
                                                                                         size_t startIndex, 
                                                                                         size_t stopIndex, 
                                                                                      __attribute__ ((unused)) size_t prediction, 
                                                                                      __attribute__ ((unused)) atn::ATNConfigSet *configs) {
    const auto text = recognizer->getTokenStream()->getText(antlr4::misc::Interval(startIndex, stopIndex));
    std::cout << "Parser reportContextSensitivity detected : " << text << std::endl;

}

void IEMLParserErrorListener::visitorError(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerError(new VisitorError(char_range, msg));
}

void IEMLParserErrorListener::parseError(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerError(new ParseError(char_range, msg));
}


void IEMLParserErrorListener::visitorWarning(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerWarning(new VisitorError(char_range, msg));
}

void IEMLParserErrorListener::parseWarning(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerWarning(new ParseError(char_range, msg));
}

