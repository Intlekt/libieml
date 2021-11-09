#include "RecognitionError.h"

#include <iostream>


using namespace ieml::parser;

void IEMLParserErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                                          size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {

    error_manager_.registerError(new SyntaxError(line, charPositionInLine, msg));
};
void IEMLParserErrorListener::reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact,
                                              const antlrcpp::BitSet &ambigAlts, atn::ATNConfigSet *configs) {


};
void IEMLParserErrorListener::reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                                          const antlrcpp::BitSet &conflictingAlts, atn::ATNConfigSet *configs) {

};
void IEMLParserErrorListener::reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                                       size_t prediction, atn::ATNConfigSet *configs) {

};

