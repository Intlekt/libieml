#include "ScriptGrammarVisitor.h"

#include "structure/script/AdditiveScript.h"
#include "structure/script/MultiplicativeScript.h"
#include "structure/script/PrimitiveScript.h"

using namespace ieml::parser;


#define RETURN_VISITOR_RESULT_MOVE(ReturnType, UNIQUE_PTR) \
  return antlrcpp::Any(VisitorResult<ReturnType>(std::move(UNIQUE_PTR)));

#define RETURN_VISITOR_RESULT_ERROR(ReturnType) \
  return antlrcpp::Any(VisitorResult<ReturnType>());


#define CHECK_SYNTAX_ERROR(ErrorListener, Context, Attribute, Message, Required) \
antlrcpp::Any t_##Attribute; \
bool valid_##Attribute = true; \
if (Context->Attribute) {\
  t_##Attribute = visit(Context->Attribute); \
  if (t_##Attribute.isNull()) { \
    ErrorListener->parseError(*charRangeFromContext(Context), Message); \
    valid_##Attribute = false; \
  }\
} else if (Required) { \
  ErrorListener->parseError(*charRangeFromContext(Context), "Missing required " #Attribute " : " Message); \
  valid_##Attribute = false; \
}

#define CHECK_SYNTAX_ERROR_LIST(ErrorListener, Context, Type, Attribute, Message) \
std::vector<std::shared_ptr<Type>> Attribute;\
__attribute__ ((unused)) bool valid_##Attribute = true; \
for (auto child: Context->Attribute) { \
  auto t_tmp = visit(child); \
  if (t_tmp.isNull()) { \
    ErrorListener->parseError(*charRangeFromContext(Context), Message); \
    valid_##Attribute = false; \
  } else { \
    auto _tmp = std::move(t_tmp.as<VisitorResult<Type>>()); \
    if (_tmp.isError())\
      valid_##Attribute = false; \
    else \
      Attribute.emplace_back(std::move(_tmp.release())); \
  }\
}

#define CAST_OR_RETURN_IF_NULL(Context, Type, Attribute, ReturnType) \
if (!valid_##Attribute) \
  RETURN_VISITOR_RESULT_ERROR(ReturnType);\
std::shared_ptr<Type> Attribute;\
if(Context->Attribute) {\
  auto _tmp = std::move(t_##Attribute.as<VisitorResult<Type>>()); \
  if (_tmp.isError())\
    RETURN_VISITOR_RESULT_ERROR(ReturnType);\
  Attribute = std::move(_tmp.release()); \
}

#define CAST_OR_RETURN_IF_NULL_LIST(Attribute, ReturnType) \
if (!valid_##Attribute) \
  RETURN_VISITOR_RESULT_ERROR(ReturnType);


/**
 * ADDITIVE SCRIPT
 */
antlrcpp::Any ScriptGrammarVisitor::visitAdditive_script__addition(ScriptParserGrammar::Additive_script__additionContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, ieml::structure::Script, children, "Expected a sum of multiplicatives scripts.");
    CAST_OR_RETURN_IF_NULL_LIST(children, ieml::structure::Script);
    
    RETURN_VISITOR_RESULT_MOVE(ieml::structure::Script, std::make_shared<ieml::structure::AdditiveScript>(children));
}

antlrcpp::Any ScriptGrammarVisitor::visitAdditive_script__remarkable(ScriptParserGrammar::Additive_script__remarkableContext *ctx) {
    RETURN_VISITOR_RESULT_MOVE(ieml::structure::Script, REMARKABLE_ADDITION(ctx->remarkable_addition->getText()[0]));
}

/**
 * MULTIPLICATIVE SCRIPT
 */
antlrcpp::Any ScriptGrammarVisitor::visitMultiplicative_script__multiplication(ScriptParserGrammar::Multiplicative_script__multiplicationContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, ieml::structure::Script, children, "Expected a substance, attribute and mode multiplicatives scripts.");
    CAST_OR_RETURN_IF_NULL_LIST(children, ieml::structure::Script);

    

    RETURN_VISITOR_RESULT_MOVE(ieml::structure::Script, std::make_shared<ieml::structure::MultiplicativeScript>(substance, attribute, mode));
}

antlrcpp::Any ScriptGrammarVisitor::visitMultiplicative_script__primitives(ScriptParserGrammar::Multiplicative_script__primitivesContext *ctx) {
    RETURN_VISITOR_RESULT_MOVE(ieml::structure::Script, PRIMITIVE(ctx->primitive->getText()[0]));

}

antlrcpp::Any ScriptGrammarVisitor::visitMultiplicative_script__remarkable(ScriptParserGrammar::Multiplicative_script__remarkableContext *ctx) {
    RETURN_VISITOR_RESULT_MOVE(ieml::structure::Script, REMARKABLE_MULTIPLICATION(ctx->remarkable_multiplication->getText()));
}
