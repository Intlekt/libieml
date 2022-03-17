#include "ScriptGrammarVisitor.h"

#include "structure/script/NullScript.h"
#include "structure/script/AdditiveScript.h"
#include "structure/script/MultiplicativeScript.h"
#include "structure/script/PrimitiveScript.h"

#include <algorithm>

using namespace ieml::parser;

#define RETURN_TYPE const ieml::structure::Script*

#define RETURN_VISITOR_RESULT_MOVE(UNIQUE_PTR) \
  return antlrcpp::Any(VisitorResult<RETURN_TYPE>(std::move(UNIQUE_PTR)));

#define RETURN_VISITOR_RESULT_ERROR() \
  return antlrcpp::Any(VisitorResult<RETURN_TYPE>());


#define REPORT_ERROR(ErrorListener, Context, Message)                 \
if (ErrorListener) {                                                  \
  ErrorListener->parseError(*charRangeFromContext(Context), Message); \
}                                                                     \


#define CHECK_SYNTAX_ERROR(ErrorListener, Context, Attribute, Message, Required) \
antlrcpp::Any t_##Attribute; \
bool valid_##Attribute = true; \
if (Context->Attribute) {\
  t_##Attribute = visit(Context->Attribute); \
  if (t_##Attribute.isNull()) { \
    REPORT_ERROR(ErrorListener, Context, Message); \
    valid_##Attribute = false; \
  }\
} else if (Required) { \
  REPORT_ERROR(ErrorListener, Context, "Missing required " #Attribute " : " Message);\
  valid_##Attribute = false; \
}

#define CHECK_SYNTAX_ERROR_LIST(ErrorListener, Context, Attribute, Message) \
std::vector<RETURN_TYPE> Attribute;\
__attribute__ ((unused)) bool valid_##Attribute = true; \
for (auto child: Context->Attribute) { \
  auto t_tmp = visit(child); \
  if (t_tmp.isNull()) { \
    REPORT_ERROR(ErrorListener, Context, Message); \
    valid_##Attribute = false; \
  } else { \
    auto _tmp = std::move(t_tmp.as<VisitorResult<RETURN_TYPE>>()); \
    if (_tmp.isError())\
      valid_##Attribute = false; \
    else \
      Attribute.emplace_back(std::move(_tmp.release())); \
  }\
}

#define CAST_OR_RETURN_IF_NULL(Context, Attribute) \
if (!valid_##Attribute) \
  RETURN_VISITOR_RESULT_ERROR();\
RETURN_TYPE Attribute = nullptr;\
if(Context->Attribute) {\
  auto _tmp = std::move(t_##Attribute.as<VisitorResult<RETURN_TYPE>>()); \
  if (_tmp.isError())\
    RETURN_VISITOR_RESULT_ERROR();\
  Attribute = std::move(_tmp.release()); \
}

#define CAST_OR_RETURN_IF_NULL_LIST(Attribute) \
if (!valid_##Attribute) \
  RETURN_VISITOR_RESULT_ERROR();


#define VISIT_SCRIPT_LAYER(layer) \
antlrcpp::Any ScriptGrammarVisitor::visitScript__layer##layer(ScriptParserGrammar::Script__layer##layer##Context *ctx) {\
    CHECK_SYNTAX_ERROR(error_listener_, ctx, script_, "Empty script", true);\
    CAST_OR_RETURN_IF_NULL(ctx, script_);\
    RETURN_VISITOR_RESULT_MOVE(script_);\
}

VISIT_SCRIPT_LAYER(0);
VISIT_SCRIPT_LAYER(1);
VISIT_SCRIPT_LAYER(2);
VISIT_SCRIPT_LAYER(3);
VISIT_SCRIPT_LAYER(4);
VISIT_SCRIPT_LAYER(5);
VISIT_SCRIPT_LAYER(6);


antlrcpp::Any ScriptGrammarVisitor::visitMult_layer0__primitive(ScriptParserGrammar::Mult_layer0__primitiveContext *ctx) {
    RETURN_VISITOR_RESULT_MOVE(register_->get_primitive(ctx->primitive->getText()[0]));
}

antlrcpp::Any ScriptGrammarVisitor::visitMult_layer0__remarkable_addition(ScriptParserGrammar::Mult_layer0__remarkable_additionContext *ctx) {
    RETURN_VISITOR_RESULT_MOVE(register_->get_remarkable_addition(ctx->remarkable_addition->getText()[0]));
}

antlrcpp::Any ScriptGrammarVisitor::visitMult_layer1__remarkable_mult(ScriptParserGrammar::Mult_layer1__remarkable_multContext *ctx) {
    std::string tag(ctx->remarkable_multiplication->getText());
    std::reverse(tag.begin(), tag.end()); 
    RETURN_VISITOR_RESULT_MOVE(register_->get_remarkable_multiplication(tag));
}

#define ADDITIVE_LAYER(layer) \
antlrcpp::Any ScriptGrammarVisitor::visitLayer##layer##__addition(ScriptParserGrammar::Layer##layer##__additionContext *ctx) { \
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, children, "Expected a sum of multiplicatives scripts.");  \
    CAST_OR_RETURN_IF_NULL_LIST(children);                                  \
                                                                            \
    std::reverse(children.begin(), children.end());                         \
    RETURN_VISITOR_RESULT_MOVE(                                             \
        register_->get_or_create_addition(                                  \
            ieml::structure::Script::Set(                                   \
                std::make_move_iterator(children.begin()),                  \
                std::make_move_iterator(children.end())                     \
            )                                                               \
        )                                                                   \
    );                                                                      \
}

#define ADDITIVE_MULT_LAYER(layer) \
antlrcpp::Any ScriptGrammarVisitor::visitLayer##layer##__script(ScriptParserGrammar::Layer##layer##__scriptContext *ctx) { \
    CHECK_SYNTAX_ERROR(error_listener_, ctx, script_, "Empty script addition", true);                       \
    CAST_OR_RETURN_IF_NULL(ctx, script_);                     \
    RETURN_VISITOR_RESULT_MOVE(script_);                        \
}

#define MULTIPLICATIVE_LAYER(layer) \
antlrcpp::Any ScriptGrammarVisitor::visitMult_layer##layer##__mult(ScriptParserGrammar::Mult_layer##layer##__multContext *ctx) {  \
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, children, "Expected a substance, attribute and mode multiplicatives scripts."); \
    CAST_OR_RETURN_IF_NULL_LIST(children);  \
                                                                \
    std::reverse(children.begin(), children.end()); \
  \
    const ieml::structure::Script *substance, *attribute, *mode;  \
    substance = children[0];  \
\
    if (children.size() > 1) {                                  \
        attribute = children[1];  \
    } else  \
        attribute = register_->get_nullscript(layer - 1); \
    \
    if (children.size() > 2) { \
        mode = children[2]; \
    } else \
        mode = register_->get_nullscript(layer - 1); \
 \
    RETURN_VISITOR_RESULT_MOVE(                       \
      register_->get_or_create_multiplication({       \
            std::move(substance),                      \
            std::move(attribute),                 \
            std::move(mode)                        \
      })                    \
    ); \
} 


ADDITIVE_LAYER(0)
ADDITIVE_MULT_LAYER(0)

ADDITIVE_LAYER(1)
ADDITIVE_MULT_LAYER(1)
MULTIPLICATIVE_LAYER(1)

ADDITIVE_LAYER(2)
ADDITIVE_MULT_LAYER(2)
MULTIPLICATIVE_LAYER(2)

ADDITIVE_LAYER(3)
ADDITIVE_MULT_LAYER(3)
MULTIPLICATIVE_LAYER(3)

ADDITIVE_LAYER(4)
ADDITIVE_MULT_LAYER(4)
MULTIPLICATIVE_LAYER(4)

ADDITIVE_LAYER(5)
ADDITIVE_MULT_LAYER(5)
MULTIPLICATIVE_LAYER(5)

ADDITIVE_LAYER(6)
ADDITIVE_MULT_LAYER(6)
MULTIPLICATIVE_LAYER(6)