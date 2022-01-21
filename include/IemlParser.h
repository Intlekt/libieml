#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "antlr4-runtime.h"
#include "iemlLexer.h"
#include "iemlParser.h"

#include "SyntaxError.h"

#include "IEMLGrammarVisitor.h"

#include "ast/Program.h"
#include "structure/path/PathTree.h"

namespace ieml {
namespace parser {


class IEMLParser {
public:

    class FileParser {
    public:
        FileParser(const std::string& file_id, const std::string& input_str, IEMLParserErrorListener* error_manager);
        ~FileParser();

        void parse();
        std::shared_ptr<Program> getProgram() {return ast_;};

        FileParser(const FileParser&) = delete;
    private:
        antlr4::ANTLRInputStream* input_ = nullptr;
        ieml_generated::iemlLexer* lexer_ = nullptr;
        ieml_generated::iemlParser* parser_ = nullptr;
        antlr4::CommonTokenStream* tokens_ = nullptr;

        std::shared_ptr<IEMLGrammarVisitor> visitor_;
        antlr4::tree::ParseTree* parseTree_ = nullptr;
        std::shared_ptr<Program> ast_;
    };


    IEMLParser(const std::vector<std::string>& file_ids, 
               const std::vector<std::string>& file_contents, 
               bool error_stdout = false);

    IEMLParser(const std::string& input_str, bool error_stdout = false) : 
        IEMLParser({""}, {input_str}, error_stdout) {};

    void parse();

    const std::vector<const SyntaxError*> getSyntaxErrors() const { return error_listener_->getSyntaxErrors(); }
    const IEMLParserErrorListener& getErrorListener() const { return *error_listener_; }
    
    std::shared_ptr<ParserContextManager> getContext() const {return context_;};

private:
    std::unordered_map<std::string, std::unique_ptr<FileParser>> file_to_parser_;
    std::vector<std::string> file_ids_;

    std::shared_ptr<IEMLParserErrorListener> error_listener_;
    std::shared_ptr<ParserContextManager> context_;
};

}}