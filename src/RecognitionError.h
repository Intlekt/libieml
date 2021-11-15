#pragma once

#include "ANTLRErrorListener.h"

#include "ast/interfaces/AST.h"


namespace ieml {
namespace parser {
    using namespace antlr4;


    class SyntaxError {
        private:
            // Token *offendingSymbol_;
            size_t line_;
            size_t charPositionInLine_;
            const std::string msg_;

        public:
            SyntaxError(size_t line, size_t charPositionInLine, const std::string& msg): line_(line), charPositionInLine_(charPositionInLine), msg_(msg) {}

            std::string to_str() const {
                return "l." + std::to_string(line_) + ":" + std::to_string(charPositionInLine_) + " " + msg_;
            }
    };


    class ErrorManager {
        private:
            std::vector<const SyntaxError*> errors_;

        public:
            void registerError(const SyntaxError* error) {
                errors_.push_back(error);
            }

            void reset() {
                for (auto * entry : errors_)
                    delete entry;
                errors_.clear();
            }

            ~ErrorManager() {
                reset();
            }

            const std::vector<const SyntaxError*> getSyntaxErrors() const { return errors_; }
    };



    class IEMLParserErrorListener: public ANTLRErrorListener {
        public:
            void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e);

            void reportAmbiguity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex, bool exact,
                                 const antlrcpp::BitSet &ambigAlts, atn::ATNConfigSet *configs);

            void reportAttemptingFullContext(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                             const antlrcpp::BitSet &conflictingAlts, atn::ATNConfigSet *configs);

            void reportContextSensitivity(Parser *recognizer, const dfa::DFA &dfa, size_t startIndex, size_t stopIndex,
                                          size_t prediction, atn::ATNConfigSet *configs);

            
            void visitorError(ieml::AST::CharRange char_range, const std::string &msg);

            const std::vector<const SyntaxError*> getSyntaxErrors() const { return error_manager_.getSyntaxErrors(); }

        private:
            ErrorManager error_manager_;
    };
}}