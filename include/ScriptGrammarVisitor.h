#pragma once

#include "ScriptParserGrammarVisitor.h"
#include "AbstractVisitor.h"
#include "SyntaxError.h"

#include "tree/ParseTreeVisitor.h"

#include "structure/script/AdditiveScript.h"
#include "structure/script/MultiplicativeScript.h"


namespace ieml::parser {

using namespace script_generated;
using namespace ieml::AST;


class ScriptGrammarVisitor: public ScriptParserGrammarVisitor, public AbstractVisitor {
public:
    ScriptGrammarVisitor(IEMLParserErrorListener* error_listener) : 
        ScriptParserGrammarVisitor(), 
        AbstractVisitor("", error_listener) // no predefined file id for Script parser
        {}

    antlrcpp::Any visit_with_offset(antlr4::tree::ParseTree *tree, const std::string& file_id, const size_t line_offset, const size_t char_offset) {
      file_id_ = file_id;
      line_offset_ = line_offset;
      char_offset_ = char_offset;
      return visit(tree);
    };

    virtual antlrcpp::Any visitAdditive_script__addition(ScriptParserGrammar::Additive_script__additionContext *context) override;
    virtual antlrcpp::Any visitAdditive_script__remarkable(ScriptParserGrammar::Additive_script__remarkableContext *context) override;

    virtual antlrcpp::Any visitMultiplicative_script__multiplication(ScriptParserGrammar::Multiplicative_script__multiplicationContext *context) override;
    virtual antlrcpp::Any visitMultiplicative_script__primitives(ScriptParserGrammar::Multiplicative_script__primitivesContext *context) override;
    virtual antlrcpp::Any visitMultiplicative_script__remarkable(ScriptParserGrammar::Multiplicative_script__remarkableContext *context) override;

private:
    IEMLParserErrorListener* error_listener_;
};
}