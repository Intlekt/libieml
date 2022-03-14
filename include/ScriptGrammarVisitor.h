#pragma once

#include "ScriptParserGrammarVisitor.h"
#include "AbstractVisitor.h"
#include "SyntaxError.h"

#include "tree/ParseTreeVisitor.h"

#include "structure/script/AdditiveScript.h"
#include "structure/script/MultiplicativeScript.h"
#include "structure/script/ScriptRegister.h"


namespace ieml::parser {

using namespace script_generated;
using namespace ieml::AST;


class ScriptGrammarVisitor: public ScriptParserGrammarVisitor, public AbstractVisitor {
public:
    ScriptGrammarVisitor(IEMLParserErrorListener* error_listener) : 
        ScriptParserGrammarVisitor(), 
        AbstractVisitor("", error_listener),
        register_(nullptr) {}

    antlrcpp::Any visit_with_offset(
            ieml::structure::ScriptRegister* reg, 
            antlr4::tree::ParseTree *tree, 
            const std::string& file_id, 
            const size_t line_offset, 
            const size_t char_offset) {
      
        register_ = reg;
        file_id_ = file_id;
        line_offset_ = line_offset;
        char_offset_ = char_offset;
        return visit(tree);
    };

    virtual antlrcpp::Any visitScript__layer0(ScriptParserGrammar::Script__layer0Context *context) override;
    virtual antlrcpp::Any visitScript__layer1(ScriptParserGrammar::Script__layer1Context *context) override;
    virtual antlrcpp::Any visitScript__layer2(ScriptParserGrammar::Script__layer2Context *context) override;
    virtual antlrcpp::Any visitScript__layer3(ScriptParserGrammar::Script__layer3Context *context) override;
    virtual antlrcpp::Any visitScript__layer4(ScriptParserGrammar::Script__layer4Context *context) override;
    virtual antlrcpp::Any visitScript__layer5(ScriptParserGrammar::Script__layer5Context *context) override;
    virtual antlrcpp::Any visitScript__layer6(ScriptParserGrammar::Script__layer6Context *context) override;

    virtual antlrcpp::Any visitMult_layer0__primitive(ScriptParserGrammar::Mult_layer0__primitiveContext *context) override;
    virtual antlrcpp::Any visitMult_layer0__remarkable_addition(ScriptParserGrammar::Mult_layer0__remarkable_additionContext *context) override;
    virtual antlrcpp::Any visitLayer0__addition(ScriptParserGrammar::Layer0__additionContext *context) override;
    virtual antlrcpp::Any visitLayer0__script(ScriptParserGrammar::Layer0__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer1__mult(ScriptParserGrammar::Mult_layer1__multContext *context) override;
    virtual antlrcpp::Any visitMult_layer1__remarkable_mult(ScriptParserGrammar::Mult_layer1__remarkable_multContext *context) override;
    virtual antlrcpp::Any visitLayer1__addition(ScriptParserGrammar::Layer1__additionContext *context) override;
    virtual antlrcpp::Any visitLayer1__script(ScriptParserGrammar::Layer1__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer2__mult(ScriptParserGrammar::Mult_layer2__multContext *context) override;
    virtual antlrcpp::Any visitLayer2__addition(ScriptParserGrammar::Layer2__additionContext *context) override;
    virtual antlrcpp::Any visitLayer2__script(ScriptParserGrammar::Layer2__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer3__mult(ScriptParserGrammar::Mult_layer3__multContext *context) override;
    virtual antlrcpp::Any visitLayer3__addition(ScriptParserGrammar::Layer3__additionContext *context) override;
    virtual antlrcpp::Any visitLayer3__script(ScriptParserGrammar::Layer3__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer4__mult(ScriptParserGrammar::Mult_layer4__multContext *context) override;
    virtual antlrcpp::Any visitLayer4__addition(ScriptParserGrammar::Layer4__additionContext *context) override;
    virtual antlrcpp::Any visitLayer4__script(ScriptParserGrammar::Layer4__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer5__mult(ScriptParserGrammar::Mult_layer5__multContext *context) override;
    virtual antlrcpp::Any visitLayer5__addition(ScriptParserGrammar::Layer5__additionContext *context) override;
    virtual antlrcpp::Any visitLayer5__script(ScriptParserGrammar::Layer5__scriptContext *context) override;

    virtual antlrcpp::Any visitMult_layer6__mult(ScriptParserGrammar::Mult_layer6__multContext *context) override;
    virtual antlrcpp::Any visitLayer6__addition(ScriptParserGrammar::Layer6__additionContext *context) override;
    virtual antlrcpp::Any visitLayer6__script(ScriptParserGrammar::Layer6__scriptContext *context) override;

private:
    ieml::structure::ScriptRegister* register_;
};
}