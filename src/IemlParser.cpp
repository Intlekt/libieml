#include "IemlParser.h"
#include "IEMLGrammarVisitor.h"

#include "ast/interfaces/AST.h"

using namespace ieml::parser;


IEMLParser::IEMLParser(const std::string& input_str, bool error_stdout) {

    input_ = new antlr4::ANTLRInputStream(input_str);
    lexer_ = new ieml_generated::iemlLexer(input_);

    errorListener_ = new IEMLParserErrorListener(error_stdout);
    lexer_->removeErrorListeners();
    lexer_->addErrorListener(errorListener_);

    tokens_ = new antlr4::CommonTokenStream(lexer_);

    parser_ = new ieml_generated::iemlParser(tokens_);
    parser_->removeErrorListeners();
    parser_->addErrorListener(errorListener_);
    
    parser_->addErrorListener(errorListener_);

    context_ = std::make_unique<ParserContext>(errorListener_);
}

IEMLParser::~IEMLParser() {
    delete input_;
    delete parser_;
    delete tokens_;
    delete lexer_;
    delete errorListener_;
}

void IEMLParser::parse() {
    if (parseTree_ != NULL) 
        return;
    parseTree_ = parser_->program();
    IEMLGrammarVisitor visitor(errorListener_);
    
    auto ast_t = visitor.visit(parseTree_);
    if (ast_t.isNull()) {
        return;
    }
    
    ast_ = std::move(ast_t.as<IEMLGrammarVisitor::VisitorResult<Program>>().release());

    if (ast_) {
        ast_.get()->check_program(*context_);
    }
}

const antlr4::tree::ParseTree* IEMLParser::getParseTree() const {
    return parseTree_;        
}

std::string IEMLParser::getParseString() const {
    return parseTree_->toStringTree(parser_);
}

const std::string IEMLParser::getASTString() const {
    if (ast_)
        return ast_->to_string();
    else
        return "";
}
