#include "IemlParser.h"
#include "IEMLGrammarVisitor.h"

#include "ast/interfaces/AST.h"

using namespace ieml::parser;


IEMLParser::FileParser::FileParser(const std::string& input_str, IEMLParserErrorListener* error_listener) {
    input_ = new antlr4::ANTLRInputStream(input_str);

    visitor_ = std::make_shared<IEMLGrammarVisitor>(error_listener);

    lexer_ = new ieml_generated::iemlLexer(input_);
    lexer_->removeErrorListeners();
    lexer_->addErrorListener(error_listener);

    tokens_ = new antlr4::CommonTokenStream(lexer_);

    parser_ = new ieml_generated::iemlParser(tokens_);
    parser_->removeErrorListeners();
    parser_->addErrorListener(error_listener);

}

IEMLParser::FileParser::~FileParser() {
    delete input_;
    delete parser_;
    delete tokens_;
    delete lexer_;
}

void IEMLParser::FileParser::parse() {
    if (parseTree_ != nullptr) 
        return;

    parseTree_ = parser_->program();
    
    auto ast_t = visitor_->visit(parseTree_);
    if (ast_t.isNull())
        return;
    
    ast_ = std::move(ast_t.as<IEMLGrammarVisitor::VisitorResult<Program>>().release());
}

IEMLParser::IEMLParser(const std::vector<std::string>& file_ids, 
                       const std::vector<std::string>& file_contents, 
                       bool error_stdout) {
    file_ids_ = file_ids;
    error_listener_ = std::make_shared<IEMLParserErrorListener>(error_stdout);

    for (size_t i = 0; i < file_ids_.size(); ++i) {
        file_to_parser_.insert({file_ids[i], std::make_unique<FileParser>(file_contents[i], error_listener_.get())});
    }

}
 
void IEMLParser::parse() {
    if (context_ != nullptr) 
        return;

    context_ = std::make_shared<ParserContext>(error_listener_.get());


    for (auto file: file_ids_) {
        auto parser = file_to_parser_.find(file);
        parser->second->parse();
        parser->second->getProgram()->check_program(*context_);
    }
}