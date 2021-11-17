#pragma once

#include <string>
#include <map>
#include <memory>
#include <regex>

#include "iemlVisitor.h"

#include "RecognitionError.h"

#include "ast/Declaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
#include "ast/Phrase.h"
#include "ast/Reference.h"
#include "ast/Word.h"
#include "ast/InflexedCategory.h"
#include "ast/ReferenceStringValue.h"
#include "ast/interfaces/ICategory.h"


#define CHECK_SYNTAX_ERROR(ErrorListener, Context, Attribute, Message, Required) \
antlrcpp::Any t_##Attribute; \
bool valid_##Attribute = true; \
if (Context->Attribute) {\
  t_##Attribute = visit(Context->Attribute); \
  if (t_##Attribute.isNull()) { \
    ErrorListener->visitorError(charRangeFromContext(Context), Message); \
    valid_##Attribute = false; \
  }\
} else if (Required) { \
  ErrorListener->visitorError(charRangeFromContext(Context), "Missing required " #Attribute " : " Message); \
  valid_##Attribute = false; \
}

#define CHECK_SYNTAX_ERROR_LIST(ErrorListener, Context, Type, Attribute, Message) \
std::vector<std::unique_ptr<Type>> Attribute;\
bool valid_##Attribute = true; \
for (auto child: Context->Attribute) { \
  auto t_tmp = visit(child); \
  if (t_tmp.isNull()) { \
    ErrorListener->visitorError(charRangeFromContext(Context), Message); \
    valid_##Attribute = false; \
  } else { \
    Attribute.emplace_back(std::move(t_tmp.as<std::unique_ptr<Type>>())); \
  }\
}


#define CAST_OR_RETURN_IF_NULL(Context, Type, Attribute) \
if (!valid_##Attribute) \
  return nullptr; \
std::unique_ptr<Type> Attribute;\
if(Context->Attribute) \
  Attribute = std::move(t_##Attribute.as<std::unique_ptr<Type>>());

#define CAST_OR_RETURN_IF_NULL_LIST(Attribute) \
if (!valid_##Attribute) \
  return nullptr; 


namespace ieml::parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlVisitor {
private:
  IEMLParserErrorListener* error_listener_;

  std::unique_ptr<CharRange> charRangeFromToken(antlr4::Token* token) const {
    return std::make_unique<CharRange>(token->getLine(), token->getLine(), token->getCharPositionInLine(), token->getCharPositionInLine() + token->getText().size());
  }
  std::unique_ptr<CharRange> charRangeFromContext(antlr4::ParserRuleContext* ctx) const {
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

public:
  IEMLGrammarVisitor(IEMLParserErrorListener* error_listener) : iemlVisitor(), error_listener_(error_listener) {}


  /**
   * PROGRAM
   */

  virtual antlrcpp::Any visitProgram(iemlParser::ProgramContext *ctx) override {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Declaration, declarations, "Invalid declaration.");
    CAST_OR_RETURN_IF_NULL_LIST(declarations);

    return std::make_unique<Program>(charRangeFromContext(ctx), std::move(declarations));
  }


  /**
   * DECLARATION
   */

  virtual antlrcpp::Any visitComponent(iemlParser::ComponentContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase definition in component declaration.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, LanguageString, language_strings, "Invalid language string.");

    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_);
    CAST_OR_RETURN_IF_NULL_LIST(language_strings);

    return std::unique_ptr<Declaration>(new ComponentDeclaration(charRangeFromContext(ctx), std::move(language_strings), std::move(phrase_)));
  }


  /**
   * PHRASE
   */

  virtual antlrcpp::Any visitPhrase__lines(iemlParser::Phrase__linesContext *ctx) override {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, PhraseLine, phrase_lines, "Invalid phrase lines.");    
    CAST_OR_RETURN_IF_NULL_LIST(phrase_lines);
    return std::unique_ptr<Phrase>(new SimplePhrase(charRangeFromContext(ctx), std::move(phrase_lines)));
  }

  virtual antlrcpp::Any visitPhrase__junction(iemlParser::Phrase__junctionContext *ctx) override {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Phrase, phrases, "Invalid phrases in phrase junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction identifier in phrase junction.", true);

    CAST_OR_RETURN_IF_NULL_LIST(phrases);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type);

    return std::unique_ptr<Phrase>(new JunctionPhrase(charRangeFromContext(ctx), std::move(phrases), std::move(junction_type)));
  }


  /**
   * PHRASELINE
   */

  virtual antlrcpp::Any visitPhrase_line__sub_phrase_line_auxiliary(iemlParser::Phrase_line__sub_phrase_line_auxiliaryContext *ctx) override {   
    
    std::unique_ptr<int> role_type = nullptr;
    if (!ctx->role_type)
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid role number.");
    else
      role_type = std::make_unique<int>(std::stoi(ctx->INTEGER()->getSymbol()->getText()));
    
    CHECK_SYNTAX_ERROR(error_listener_, ctx, sub_phrase, "Invalid sub phrase line : invalid auxiliary, inflexion, category or references.", true);

    CAST_OR_RETURN_IF_NULL(ctx, AuxiliarySubPhraseLine, sub_phrase);
    if (!role_type)
      return nullptr;

    bool accentuation = ctx->accentuation;

    return std::unique_ptr<PhraseLine>(new SimplePhraseLine(charRangeFromContext(ctx), 
                                       std::move(role_type), 
                                       accentuation, 
                                       std::move(sub_phrase)));
  }

  virtual antlrcpp::Any visitPhrase_line__jonction_auxiliary(iemlParser::Phrase_line__jonction_auxiliaryContext *ctx) override {
    std::unique_ptr<int> role_type = nullptr;
    if (!ctx->role_type)
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid role number.");
    else
      role_type = std::make_unique<int>(std::stoi(ctx->INTEGER()->getSymbol()->getText()));

    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, AuxiliarySubPhraseLine, sub_phrases, "Invalid sub phrase in phrase line junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction identifier in phrase line junction.", true);

    bool accentuation = ctx->accentuation;

    CAST_OR_RETURN_IF_NULL_LIST(sub_phrases);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type);

    if (!role_type)
      return nullptr;

    return std::unique_ptr<PhraseLine>(new JunctionPhraseLine(charRangeFromContext(ctx),
                                                              std::move(sub_phrases),
                                                              std::move(junction_type),
                                                              std::move(role_type), 
                                                              accentuation));
  }


  /**
   * SUB PHRASELINE with AUXILIARY
   */

  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__sub_phrase_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__sub_phrase_no_auxiliaryContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary, "Invalid auxiliary identifier.", true);
    CHECK_SYNTAX_ERROR(error_listener_, ctx, inflexed_category_, "Missing a category : an identifier, a phrase or a word.", true);

    CAST_OR_RETURN_IF_NULL(ctx, InflexedCategory, inflexed_category_);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, auxiliary);

    return std::unique_ptr<AuxiliarySubPhraseLine>(
      new SimpleAuxiliarySubPhraseLine(charRangeFromContext(ctx), 
                                       std::move(auxiliary), 
                                       std::move(inflexed_category_)));
  }

  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__jonction_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__jonction_no_auxiliaryContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, auxiliary, "Invalid auxiliary identifier.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, InflexedCategory, inflexed_categories, "Invalid inflexed categories in auxiliarized phrase line junction.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, junction_type, "Invalid junction type identifier.", true);
    
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, auxiliary);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, junction_type);
    CAST_OR_RETURN_IF_NULL_LIST(inflexed_categories)

    return std::unique_ptr<AuxiliarySubPhraseLine>(
      new JunctionAuxiliarySubPhraseLine(charRangeFromContext(ctx), 
                                         std::move(auxiliary), 
                                         std::move(inflexed_categories),
                                         std::move(junction_type)));
  }


  /**
   * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
   */

  virtual antlrcpp::Any visitInflexed_category(iemlParser::Inflexed_categoryContext *ctx) override {
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Identifier, inflexions, "Invalid inflexion in inflexed category.");
    CHECK_SYNTAX_ERROR(error_listener_, ctx, category_, "Missing a category : an identifier, a phrase or a word.", true);
    CHECK_SYNTAX_ERROR_LIST(error_listener_, ctx, Reference, references, "Invalid reference.");

    CAST_OR_RETURN_IF_NULL_LIST(inflexions);
    CAST_OR_RETURN_IF_NULL(ctx, ICategory, category_);
    CAST_OR_RETURN_IF_NULL_LIST(references);

    return std::make_unique<InflexedCategory>(charRangeFromContext(ctx), std::move(inflexions), std::move(category_), std::move(references));
  }


  /**
   *  CATEGORY
   */

  virtual antlrcpp::Any visitCategory__identifier(iemlParser::Category__identifierContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for a category.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_);
    return std::unique_ptr<ICategory>(std::move(identifier_));
  }

  virtual antlrcpp::Any visitCategory__phrase(iemlParser::Category__phraseContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for a category.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_);
    return std::unique_ptr<ICategory>(std::move(phrase_));
  }

  virtual antlrcpp::Any visitCategory__word(iemlParser::Category__wordContext *ctx) override {
    if (!ctx->word) {
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid word for a category.");
      return nullptr;
    }

    std::string word_str_quoted = ctx->word->getText();
    std::string word_str = word_str_quoted.substr(1, word_str_quoted.size() - 2);
    
    return std::unique_ptr<ICategory>(new Word(charRangeFromContext(ctx), word_str));
  }


  /**
   *  LANGUAGE STRING
   */

  virtual antlrcpp::Any visitLanguage_string(iemlParser::Language_stringContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, language, "Invalid language identifier for language string.", true);
    CHECK_SYNTAX_ERROR(error_listener_, ctx, value, "Invalid identifier value for language string.", true);

    CAST_OR_RETURN_IF_NULL(ctx, Identifier, language)
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, value)

    return std::make_unique<LanguageString>(charRangeFromContext(ctx), 
                                            std::move(language), 
                                            std::move(value));
  }


  /**
   *  IDENTIFIER
   */

  virtual antlrcpp::Any visitIdentifier(iemlParser::IdentifierContext *ctx) override {
    if (ctx->identifiers.empty()) {
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid identifier : empty identifier.");
      return nullptr;
    }

    std::ostringstream os;
    bool first = true;
    for (antlr4::Token* token : ctx->identifiers) {
      if (first) first = false;
      else os << " ";
      
      os << token->getText();
    }

    return std::make_unique<Identifier>(charRangeFromContext(ctx), os.str());
  }


  /**
   *  REFERENCE
   */

  virtual antlrcpp::Any visitReference(iemlParser::ReferenceContext *ctx) override {
    std::unique_ptr<int> ref_id = nullptr;
    if (ctx->id) {
      int i = std::stoi(ctx->id->getText());
      ref_id = std::move(std::make_unique<int>(i));
    }

    CHECK_SYNTAX_ERROR(error_listener_, ctx, value, "Invalid value in reference.", true);

    if (!ctx->data_type) {
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid datatype identifier.");
      return nullptr;
    }

    CAST_OR_RETURN_IF_NULL(ctx, IReferenceValue, value);
    auto data_type = std::move(std::make_unique<Identifier>(charRangeFromToken(ctx->data_type), ctx->data_type->getText()));

    return std::make_unique<Reference>(charRangeFromContext(ctx),
                                       std::move(ref_id),
                                       std::move(data_type),
                                       std::move(value));
  }


  /**
   *  REFERENCE VALUE
   */

  virtual antlrcpp::Any visitReference_value__identifier(iemlParser::Reference_value__identifierContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, identifier_, "Invalid identifier for reference value.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Identifier, identifier_);

    return static_cast<std::unique_ptr<IReferenceValue>>(std::move(identifier_));
  }

  virtual antlrcpp::Any visitReference_value__STRING(iemlParser::Reference_value__STRINGContext *ctx) override {
    if (!ctx->value) {
      error_listener_->visitorError(charRangeFromContext(ctx), "Invalid string for reference value.");
      return nullptr;
    }
    return std::unique_ptr<IReferenceValue>(new ReferenceStringValue(charRangeFromContext(ctx), ctx->value->getText()));
  }

  virtual antlrcpp::Any visitReference_value__phrase(iemlParser::Reference_value__phraseContext *ctx) override {
    CHECK_SYNTAX_ERROR(error_listener_, ctx, phrase_, "Invalid phrase for reference value.", true);
    CAST_OR_RETURN_IF_NULL(ctx, Phrase, phrase_);
    return static_cast<std::unique_ptr<IReferenceValue>>(std::move(phrase_));
  }
};

}
