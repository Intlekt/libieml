#include "IEMLGrammarVisitor.h"

#include "ast/Declaration.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
#include "ast/Phrase.h"
#include "ast/Reference.h"
#include "ast/Word.h"
#include "ast/InflexedCategory.h"
#include "ast/ReferenceStringValue.h"
#include "ast/interfaces/ICategory.h"

#define RETURN_VISITOR_RESULT(ReturnType, DerivedType, args...) \
  return (VisitorResult<ReturnType>(std::make_unique<DerivedType>(charRangeFromContext(ctx), args)));

#define RETURN_VISITOR_RESULT_MOVE(ReturnType, UNIQUE_PTR) \
  return (VisitorResult<ReturnType>(std::move(UNIQUE_PTR)));

#define RETURN_VISITOR_RESULT_ERROR(ReturnType) \
  return (VisitorResult<ReturnType>());


#define CHECK_SYNTAX_ERROR(ErrorListener, Context, Attribute, Message, Required) \
antlrcpp::Any t_##Attribute; \
bool valid_##Attribute = true; \
if (Context->Attribute) {\
  t_##Attribute = visit(Context->Attribute); \
  if (t_##Attribute.isNull()) { \
    ErrorListener->visitorError(*charRangeFromContext(Context), Message); \
    valid_##Attribute = false; \
  }\
} else if (Required) { \
  ErrorListener->visitorError(*charRangeFromContext(Context), "Missing required " #Attribute " : " Message); \
  valid_##Attribute = false; \
}

#define CHECK_SYNTAX_ERROR_LIST(ErrorListener, Context, Type, Attribute, Message) \
std::vector<std::unique_ptr<Type>> Attribute;\
bool valid_##Attribute = true; \
for (auto child: Context->Attribute) { \
  auto t_tmp = visit(child); \
  if (t_tmp.isNull()) { \
    ErrorListener->visitorError(*charRangeFromContext(Context), Message); \
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
std::unique_ptr<Type> Attribute;\
if(Context->Attribute) {\
  auto _tmp = std::move(t_##Attribute.as<VisitorResult<Type>>()); \
  if (_tmp.isError())\
    RETURN_VISITOR_RESULT_ERROR(ReturnType);\
  Attribute = std::move(_tmp.release()); \
}

#define CAST_OR_RETURN_IF_NULL_LIST(Attribute, ReturnType) \
if (!valid_##Attribute) \
  RETURN_VISITOR_RESULT_ERROR(ReturnType);

namespace ieml::parser {

  std::unique_ptr<CharRange> IEMLGrammarVisitor::charRangeFromToken(antlr4::Token* token) const {
    return std::make_unique<CharRange>(token->getLine(), token->getLine(), token->getCharPositionInLine(), token->getCharPositionInLine() + token->getText().size());
  }

  std::unique_ptr<CharRange> IEMLGrammarVisitor::charRangeFromContext(antlr4::ParserRuleContext* ctx) const {
    antlr4::Token* start = ctx->getStart();
    antlr4::Token* end   = ctx->getStop();

    size_t line_s, line_e, char_s, char_e;

    // these tests are for the case if start is after end for a empty production rule (see documentation for Token.getStart())
    if (start->getLine() < end->getLine()) {
      line_s = start->getLine();
      line_e = end->getLine();
      char_s = start->getCharPositionInLine();
      char_e = end->getCharPositionInLine();
    } else if (start->getLine() > end->getLine()) {
      line_e = start->getLine();
      line_s = end->getLine();
      char_e = start->getCharPositionInLine();
      char_s = end->getCharPositionInLine();
    } else {
      line_s = line_e = start->getLine();
      if (start->getCharPositionInLine() <= end->getCharPositionInLine()) {
        char_s = start->getCharPositionInLine();
        char_e = end->getCharPositionInLine();
      } else {
        char_e = start->getCharPositionInLine();
        char_s = end->getCharPositionInLine();
      }
    } 
    
    return std::make_unique<CharRange>(line_s, line_e, char_s, char_e);
  }


  /**
   * PROGRAM
   */

  antlrcpp::Any IEMLGrammarVisitor::visitProgram(iemlParser::ProgramContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Declaration, declarations, "Invalid declaration.");
    CAST_OR_RETURN_IF_NULL_LIST(declarations, Program);

    RETURN_VISITOR_RESULT(Program, Program, std::move(declarations));
  }


  /**
   * DECLARATION
   */

  antlrcpp::Any IEMLGrammarVisitor::visitComponentDeclaration(iemlParser::ComponentDeclarationContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in component declaration.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, Declaration);
    CAST_OR_RETURN_IF_NULL_LIST(language_strings, Declaration);

    RETURN_VISITOR_RESULT(Declaration, ComponentDeclaration, std::move(language_strings), std::move(phrase_));
  }

  antlrcpp::Any IEMLGrammarVisitor::visitNodeDeclaration(iemlParser::NodeDeclarationContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in node declaration.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, Declaration);
    CAST_OR_RETURN_IF_NULL_LIST(language_strings, Declaration);

    RETURN_VISITOR_RESULT(Declaration, NodeDeclaration, std::move(language_strings), std::move(phrase_));
  }

  antlrcpp::Any IEMLGrammarVisitor::visitWordDeclaration(iemlParser::WordDeclarationContext *ctx) {
    if (!ctx->word) {
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid word for a word declaration.");
      RETURN_VISITOR_RESULT_ERROR(Declaration);
    }

    std::string word_str_quoted = ctx->word->getText();
    std::string word_str = word_str_quoted.substr(1, word_str_quoted.size() - 2);
    
    RETURN_VISITOR_RESULT(Declaration, 
                          WordDeclaration, 
                          std::make_unique<Word>(charRangeFromToken(ctx->word), word_str));
  }

  antlrcpp::Any IEMLGrammarVisitor::visitInflexingDeclaration(iemlParser::InflexingDeclarationContext *context) {
    
  }

  antlrcpp::Any IEMLGrammarVisitor::visitAuxiliaryDeclaration(iemlParser::AuxiliaryDeclarationContext *context) {

  }
  
  antlrcpp::Any IEMLGrammarVisitor::visitJunctionDeclaration(iemlParser::JunctionDeclarationContext *context) {

  }

  antlrcpp::Any IEMLGrammarVisitor::visitLanguageDeclaration(iemlParser::LanguageDeclarationContext *ctx) {

  }

  /**
   * PHRASE
   */

  antlrcpp::Any IEMLGrammarVisitor::visitPhrase__lines(iemlParser::Phrase__linesContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, PhraseLine, phrase_lines, "Invalid phrase lines.");    
    CAST_OR_RETURN_IF_NULL_LIST(phrase_lines, Phrase);

    RETURN_VISITOR_RESULT(Phrase, SimplePhrase, std::move(phrase_lines));
  }

  antlrcpp::Any IEMLGrammarVisitor::visitPhrase__junction(iemlParser::Phrase__junctionContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Phrase, phrases, "Invalid phrases in phrase junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction identifier in phrase junction.", true);

    CAST_OR_RETURN_IF_NULL_LIST(phrases, Phrase);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type, Phrase);

    RETURN_VISITOR_RESULT(Phrase, JunctionPhrase, std::move(phrases), std::move(junction_type));
  }


  /**
   * PHRASELINE
   */

  antlrcpp::Any IEMLGrammarVisitor::visitPhrase_line__sub_phrase_line_auxiliary(iemlParser::Phrase_line__sub_phrase_line_auxiliaryContext *ctx) {
    std::unique_ptr<int> role_type = nullptr;
    if (!ctx->role_type)
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid role number.");
    else
      role_type = std::make_unique<int>(std::stoi(ctx->INTEGER()->getSymbol()->getText()));
    
    CHECK_SYNTAX_ERROR(error_listener_, ctx, sub_phrase, "Invalid sub phrase line : invalid auxiliary, inflexion, category or references.", true);

    CAST_OR_RETURN_IF_NULL(ctx, AuxiliarySubPhraseLine, sub_phrase, PhraseLine);
    
    if (!role_type)
      RETURN_VISITOR_RESULT_ERROR(PhraseLine);

    bool accentuation = ctx->accentuation;

    RETURN_VISITOR_RESULT(PhraseLine, SimplePhraseLine, std::move(role_type), accentuation, std::move(sub_phrase))
  }

  antlrcpp::Any IEMLGrammarVisitor::visitPhrase_line__jonction_auxiliary(iemlParser::Phrase_line__jonction_auxiliaryContext *ctx) {
    std::unique_ptr<int> role_type = nullptr;
    if (!ctx->role_type)
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid role number.");
    else
      role_type = std::make_unique<int>(std::stoi(ctx->role_type->getText()));

    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, AuxiliarySubPhraseLine, sub_phrases, "Invalid sub phrase in phrase line junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction identifier in phrase line junction.", true);

    bool accentuation = ctx->accentuation;

    CAST_OR_RETURN_IF_NULL_LIST(sub_phrases, PhraseLine);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type, PhraseLine);

    if (!role_type)
      RETURN_VISITOR_RESULT_ERROR(PhraseLine);
    
    RETURN_VISITOR_RESULT(PhraseLine, JunctionPhraseLine, std::move(sub_phrases), std::move(junction_type), std::move(role_type), accentuation);
  }


  /**
   * SUB PHRASELINE with AUXILIARY
   */

  antlrcpp::Any IEMLGrammarVisitor::visitSub_phrase_line_auxiliary__sub_phrase_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__sub_phrase_no_auxiliaryContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary, "Invalid auxiliary identifier.", false);
    CHECK_SYNTAX_ERROR(error_listener_, ctx, inflexed_category_, "Missing a category : an identifier, a phrase or a word.", true);

    CAST_OR_RETURN_IF_NULL(ctx, InflexedCategory, inflexed_category_, AuxiliarySubPhraseLine);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, auxiliary, AuxiliarySubPhraseLine);
    
    RETURN_VISITOR_RESULT(AuxiliarySubPhraseLine, SimpleAuxiliarySubPhraseLine, std::move(auxiliary), std::move(inflexed_category_));
  }

  antlrcpp::Any IEMLGrammarVisitor::visitSub_phrase_line_auxiliary__jonction_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__jonction_no_auxiliaryContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary, "Invalid auxiliary identifier.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, InflexedCategory, inflexed_categories, "Invalid inflexed categories in auxiliarized phrase line junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction type identifier.", true);
    
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, auxiliary, AuxiliarySubPhraseLine);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type, AuxiliarySubPhraseLine);
    CAST_OR_RETURN_IF_NULL_LIST(inflexed_categories, AuxiliarySubPhraseLine)

    RETURN_VISITOR_RESULT(AuxiliarySubPhraseLine, JunctionAuxiliarySubPhraseLine, std::move(auxiliary), std::move(inflexed_categories), std::move(junction_type));
  }


  /**
   * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
   */

  antlrcpp::Any IEMLGrammarVisitor::visitInflexed_category(iemlParser::Inflexed_categoryContext *ctx) {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Identifier, inflexions, "Invalid inflexion in inflexed category.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a category : an identifier, a phrase or a word.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Reference, references, "Invalid reference.");

    CAST_OR_RETURN_IF_NULL_LIST(inflexions, InflexedCategory);
    CAST_OR_RETURN_IF_NULL(ctx, ICategory, category_, InflexedCategory);
    CAST_OR_RETURN_IF_NULL_LIST(references, InflexedCategory);

    RETURN_VISITOR_RESULT(InflexedCategory, InflexedCategory, std::move(inflexions), std::move(category_), std::move(references));
  }


  /**
   *  CATEGORY
   */

  antlrcpp::Any IEMLGrammarVisitor::visitCategory__identifier(iemlParser::Category__identifierContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for a category.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, ICategory);

    RETURN_VISITOR_RESULT_MOVE(ICategory, identifier_);
  }

  antlrcpp::Any IEMLGrammarVisitor::visitCategory__phrase(iemlParser::Category__phraseContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for a category.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, ICategory);

    RETURN_VISITOR_RESULT_MOVE(ICategory, phrase_);
  }

  antlrcpp::Any IEMLGrammarVisitor::visitCategory__word(iemlParser::Category__wordContext *ctx) {
    if (!ctx->word) {
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid word for a category.");
      RETURN_VISITOR_RESULT_ERROR(ICategory);
    }

    std::string word_str_quoted = ctx->word->getText();
    std::string word_str = word_str_quoted.substr(1, word_str_quoted.size() - 2);
    
    RETURN_VISITOR_RESULT(ICategory, Word, word_str);
  }


  /**
   *  LANGUAGE STRING
   */

  antlrcpp::Any IEMLGrammarVisitor::visitLanguage_string(iemlParser::Language_stringContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, language, "Invalid language identifier for language string.", true);
    CHECK_SYNTAX_ERROR(error_listener_, ctx, value, "Invalid identifier value for language string.", true);

    CAST_OR_RETURN_IF_NULL(ctx, Identifier, language, LanguageString)
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, value, LanguageString)

    RETURN_VISITOR_RESULT(LanguageString, LanguageString, std::move(language), std::move(value))
  }


  /**
   *  IDENTIFIER
   */

  antlrcpp::Any IEMLGrammarVisitor::visitIdentifier(iemlParser::IdentifierContext *ctx) {
    if (ctx->identifiers.empty()) {
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid identifier : empty identifier.");
      RETURN_VISITOR_RESULT_ERROR(Identifier);
    }

    std::ostringstream os;
    bool first = true;
    for (antlr4::Token* token : ctx->identifiers) {
      if (first) first = false;
      else os << " ";
      
      os << token->getText();
    }

    RETURN_VISITOR_RESULT(Identifier, Identifier, os.str());
  }


  /**
   *  REFERENCE
   */

  antlrcpp::Any IEMLGrammarVisitor::visitReference(iemlParser::ReferenceContext *ctx) {
    std::unique_ptr<int> ref_id = nullptr;
    if (ctx->id) {
      int i = std::stoi(ctx->id->getText());
      ref_id = std::move(std::make_unique<int>(i));
    }

    CHECK_SYNTAX_ERROR(error_listener_, ctx, value, "Invalid value in reference.", true);

    if (!ctx->data_type) {
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid datatype identifier.");
      RETURN_VISITOR_RESULT_ERROR(Reference);
    }

    CAST_OR_RETURN_IF_NULL(ctx, IReferenceValue, value, Reference);
    auto data_type = std::move(std::make_unique<Identifier>(charRangeFromToken(ctx->data_type), ctx->data_type->getText()));

    RETURN_VISITOR_RESULT(Reference, Reference, std::move(ref_id), std::move(data_type), std::move(value));
  }


  /**
   *  REFERENCE VALUE
   */

  antlrcpp::Any IEMLGrammarVisitor::visitReference_value__identifier(iemlParser::Reference_value__identifierContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for reference value.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_, IReferenceValue);

    RETURN_VISITOR_RESULT_MOVE(IReferenceValue, identifier_);
  }

  antlrcpp::Any IEMLGrammarVisitor::visitReference_value__STRING(iemlParser::Reference_value__STRINGContext *ctx) {
    if (!ctx->value) {
      error_listener_->visitorError(*charRangeFromContext(ctx), "Invalid string for reference value.");
      RETURN_VISITOR_RESULT_ERROR(IReferenceValue);
    }

    RETURN_VISITOR_RESULT(IReferenceValue, ReferenceStringValue, ctx->value->getText());
  }

  antlrcpp::Any IEMLGrammarVisitor::visitReference_value__phrase(iemlParser::Reference_value__phraseContext *ctx) {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for reference value.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_, IReferenceValue);

    RETURN_VISITOR_RESULT_MOVE(IReferenceValue, phrase_);
  }


}