#include "ScriptParser.h"
#include "ScriptGrammarVisitor.h"


using namespace ieml::parser;

ScriptParser::ScriptParser(const std::string& file_id, IEMLParserErrorListener* error_listener) {
    visitor_ = std::make_shared<ScriptGrammarVisitor>(error_listener);
    const auto input = new antlr4::ANTLRInputStream(std::string(""));

    auto antlr_listener = error_listener->getANTLR4ErrorListener(file_id);
    lexer_ = new script_generated::ScriptLexerGrammar(input);
    lexer_->removeErrorListeners();
    lexer_->addErrorListener(antlr_listener);

    const auto tokens = new antlr4::CommonTokenStream(lexer_);

    parser_ = new script_generated::ScriptParserGrammar(tokens);
    parser_->removeErrorListeners();
    parser_->addErrorListener(antlr_listener);
    
    // use SSL prediction for parser
    // I wrote the grammar so it not need context information, speeding the parser by x10
    parser_->getInterpreter<atn::ParserATNSimulator>()->setPredictionMode(atn::PredictionMode::SLL);
}

ScriptParser::~ScriptParser() {
    delete input_;
    delete parser_;
    delete lexer_;
}

const ieml::structure::Script* ScriptParser::parse(
        ieml::structure::ScriptRegister* reg,
        const std::string& input_str, 
        const std::string& file_id, 
        const size_t line_offset, 
        const size_t char_offset) {
    
    std::string reversed(input_str);
    // parse the string reversed
    std::reverse(reversed.begin(), reversed.end());

    const auto input = new antlr4::ANTLRInputStream(reversed);
    lexer_->setInputStream(input);
    const auto tokens = new antlr4::CommonTokenStream(lexer_);
    parser_->setTokenStream(tokens);

    const auto parse_tree = parser_->script();
    
    const auto script = std::move(visitor_->visit_with_offset(reg, parse_tree, file_id, line_offset, char_offset)
                                          .as<ScriptGrammarVisitor::VisitorResult<const ieml::structure::Script*>>());
    if (script.isError())
        return nullptr;
    
    return script.value();
}

