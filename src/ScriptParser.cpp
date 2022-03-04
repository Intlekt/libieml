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
}

ScriptParser::~ScriptParser() {
    delete input_;
    delete parser_;
    delete lexer_;
}

std::shared_ptr<ieml::structure::Script> ScriptParser::parse(const std::string& input_str, const std::string& file_id, const size_t line_offset, const size_t char_offset) {
    const auto input = new antlr4::ANTLRInputStream(input_str);
    lexer_->setInputStream(input);
    const auto tokens = new antlr4::CommonTokenStream(lexer_);
    parser_->setTokenStream(tokens);

    const auto parse_tree = parser_->additive_script();
    
    const auto script = visitor_->visit_with_offset(parse_tree, file_id, line_offset, char_offset);
    if (script.isNull())
        return nullptr;
    
    return std::move(script.as<std::shared_ptr<ieml::structure::Script>>());
}

