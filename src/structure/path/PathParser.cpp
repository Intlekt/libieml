#include "structure/path/PathParser.h"
#include "ast/Path.h"
#include "IEMLGrammarVisitor.h"
#include "antlr4-runtime.h"
#include "IEMLLexerGrammar.h"
#include "IEMLParserGrammar.h"



std::shared_ptr<ieml::structure::PathTree> ieml::parser::parsePath(parser::ParserContextManager& ctx, const std::string& input_str, bool error_stdout) {
    antlr4::ANTLRInputStream input(input_str);
    IEMLParserErrorListener error_listener(error_stdout);
    auto antlr_listener = error_listener.getANTLR4ErrorListener("");

    ieml_generated::IEMLLexerGrammar lexer(&input);
    lexer.removeErrorListeners();
    lexer.addErrorListener(antlr_listener);

    antlr4::CommonTokenStream tokens(&lexer);

    ieml_generated::IEMLParserGrammar parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(antlr_listener);

    auto parse_tree = parser.path();

    IEMLGrammarVisitor visitor("a", &error_listener);

    auto ast_t = visitor.visit(parse_tree);
    if (ast_t.isNull())
        return nullptr;

    const auto& ast = ast_t.as<IEMLGrammarVisitor::VisitorResult<std::shared_ptr<ieml::AST::Path>>>().release();
    if (ast == nullptr) return nullptr;
    return ast->check_path(ctx);
}
