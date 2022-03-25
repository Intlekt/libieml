#include "IemlParser.h"
#include "IEMLGrammarVisitor.h"

#include "ast/interfaces/AST.h"

using namespace ieml::parser;


const std::string IEMLParser::default_file_id = "__default__";

IEMLParser::FileParser::FileParser(const std::string& file_id, const std::string& input_str, IEMLParserErrorListener* error_listener) {
    input_ = new antlr4::ANTLRInputStream(input_str);

    visitor_ = std::make_shared<IEMLGrammarVisitor>(file_id, error_listener);

    auto antlr_listener = error_listener->getANTLR4ErrorListener(file_id);
    lexer_ = new ieml_generated::IEMLLexerGrammar(input_);
    lexer_->removeErrorListeners();
    lexer_->addErrorListener(antlr_listener);

    tokens_ = new antlr4::CommonTokenStream(lexer_);

    parser_ = new ieml_generated::IEMLParserGrammar(tokens_);
    parser_->removeErrorListeners();
    parser_->addErrorListener(antlr_listener);

    // use SSL prediction for parser
    // I wrote the grammar so it not need context information, speeding the parser by x10
    // But if there is an issue, you should try other atn::PredictionMode for more context awareness
    parser_->getInterpreter<atn::ParserATNSimulator>()->setPredictionMode(atn::PredictionMode::SLL);
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
    ast_ = std::move(visitor_->visit(parseTree_)
                             .as<IEMLGrammarVisitor::VisitorResult<std::shared_ptr<Program>>>()
                             .release());
}


IEMLParser::IEMLParser(const std::vector<std::string>& file_ids, 
                       const std::vector<std::string>& file_contents, 
                       bool error_stdout) {
    file_ids_ = file_ids;
    error_listener_ = std::make_shared<IEMLParserErrorListener>(error_stdout);

    for (size_t i = 0; i < file_ids_.size(); ++i) {
        file_to_parser_.insert({file_ids[i], std::make_unique<FileParser>(file_ids[i], file_contents[i], error_listener_.get())});
    }

}
 
void IEMLParser::parse() {
    if (context_ != nullptr) 
        return;

    context_ = std::make_shared<ParserContextManager>(error_listener_.get());


    for (auto file: file_ids_) {
        auto parser = file_to_parser_.find(file);
        parser->second->parse();
        
        context_->resetLanguage();
        parser->second->getProgram()->check_program(*context_);
    }

    context_->registerLinkInstances();
}
