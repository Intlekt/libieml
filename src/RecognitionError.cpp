#include "RecognitionError.h"
#include "Token.h"


#include <iostream>


using namespace ieml::parser;
using json = nlohmann::json;

void IEMLParserErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                                          size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {

    error_manager_.registerError(new SyntaxError(
        ieml::AST::CharRange(
            line, 
            line, 
            charPositionInLine, 
            charPositionInLine + offendingSymbol->getText().size()
        ), msg));
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

void IEMLParserErrorListener::visitorError(const ieml::AST::CharRange& char_range, const std::string &msg) {
    error_manager_.registerError(new SyntaxError(char_range, msg));
};

json SyntaxError::toJson() const {
    return {
        {"range", char_range_.toJson()},
        {"message", msg_}
    };
}

json IEMLParserErrorListener::toJson() const {
    json error_list = json::array();
    for (auto& error: error_manager_.getSyntaxErrors()) {
        error_list.push_back(error->toJson());
    }
    return error_list;
}
