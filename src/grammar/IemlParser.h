#pragma once

#include "antlr4-runtime.h"
#include "iemlLexer.h"
#include "iemlParser.h"

#include "RecognitionError.h"

namespace ieml {
namespace parser {


class IEMLParser {
private:
    antlr4::ANTLRInputStream* input_ = NULL;
    IEMLParserErrorListener* errorListener_ = NULL;

    ieml_generated::iemlLexer* lexer_ = NULL;
    ieml_generated::iemlParser* parser_ = NULL;
    antlr4::CommonTokenStream* tokens_ = NULL;

    antlr4::tree::ParseTree* parseTree_ = NULL;

public:
    explicit IEMLParser(const std::string& input_str);

    ~IEMLParser();

    void parse();
    const antlr4::tree::ParseTree* getParseTree() const;
    std::string getParseString() const;


    const std::vector<const SyntaxError*> getSyntaxErrors() const { return errorListener_->getSyntaxErrors(); }


};

}}