#pragma once

#include <string>
#include <map>
#include <memory>
#include <regex>

#include "iemlVisitor.h"
#include "RecognitionError.h"
#include "ast/CharRange.h"


namespace ieml::parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlVisitor {
private:
  IEMLParserErrorListener* error_listener_;

  std::shared_ptr<CharRange> charRangeFromToken(antlr4::Token* token) const ;
  std::shared_ptr<CharRange> charRangeFromContext(antlr4::ParserRuleContext* ctx) const ;

public:
  template<class T>
  class VisitorResult {
  public:
    VisitorResult(): is_error_(true) {}
    VisitorResult(std::shared_ptr<T>&& value): is_error_(false), value_(std::move(value)) {}

    VisitorResult(const VisitorResult& vr) = delete;
    VisitorResult(VisitorResult&& vr) noexcept : is_error_(vr.is_error_), value_(std::move(vr.value_)) {};
    VisitorResult& operator= (const VisitorResult&) = delete;
    VisitorResult& operator= (VisitorResult&&) = default;

    bool isError() {return is_error_;};

    std::shared_ptr<T>&& release() { return std::move(value_); }

    const T& value() {return *value_;}

  private:
    bool is_error_;
    std::shared_ptr<T> value_;
  };

  IEMLGrammarVisitor(IEMLParserErrorListener* error_listener) : iemlVisitor(), error_listener_(error_listener) {}

  /**
   * PROGRAM
   */
  virtual antlrcpp::Any visitProgram(iemlParser::ProgramContext *ctx) override;

  /**
   * DECLARATION
   */
  virtual antlrcpp::Any visitComponentDeclaration(iemlParser::ComponentDeclarationContext *ctx) override;
  virtual antlrcpp::Any visitNodeDeclaration(iemlParser::NodeDeclarationContext *context) override;
  virtual antlrcpp::Any visitWordDeclaration(iemlParser::WordDeclarationContext *context) override;
  virtual antlrcpp::Any visitInflectionDeclaration(iemlParser::InflectionDeclarationContext *context) override;
  virtual antlrcpp::Any visitAuxiliaryDeclaration(iemlParser::AuxiliaryDeclarationContext *context) override;
  virtual antlrcpp::Any visitJunctionDeclaration(iemlParser::JunctionDeclarationContext *context) override;
  virtual antlrcpp::Any visitLanguageDeclaration(iemlParser::LanguageDeclarationContext *context) override;

  /**
   * PHRASE
   */
  virtual antlrcpp::Any visitPhrase__lines(iemlParser::Phrase__linesContext *ctx) override;
  virtual antlrcpp::Any visitPhrase__junction(iemlParser::Phrase__junctionContext *ctx) override;

  /**
   * PHRASELINE
   */
  virtual antlrcpp::Any visitPhrase_line__sub_phrase_line_auxiliary(iemlParser::Phrase_line__sub_phrase_line_auxiliaryContext *ctx) override;
  virtual antlrcpp::Any visitPhrase_line__jonction_auxiliary(iemlParser::Phrase_line__jonction_auxiliaryContext *ctx) override;

  /**
   * SUB PHRASELINE with AUXILIARY
   */
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__sub_phrase_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__sub_phrase_no_auxiliaryContext *ctx) override;
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__jonction_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__jonction_no_auxiliaryContext *ctx) override;

  /**
   * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
   */
  virtual antlrcpp::Any visitInflexed_category(iemlParser::Inflexed_categoryContext *ctx) override;

  /**
   *  CATEGORY
   */
  virtual antlrcpp::Any visitCategory__identifier(iemlParser::Category__identifierContext *ctx) override;
  virtual antlrcpp::Any visitCategory__phrase(iemlParser::Category__phraseContext *ctx) override;
  virtual antlrcpp::Any visitCategory__word(iemlParser::Category__wordContext *ctx) override;

  /**
   *  LANGUAGE STRING
   */
  virtual antlrcpp::Any visitLanguage_string(iemlParser::Language_stringContext *ctx) override;

  /**
   *  IDENTIFIER
   */
  virtual antlrcpp::Any visitIdentifier(iemlParser::IdentifierContext *ctx) override;

  /**
   *  REFERENCE
   */
  virtual antlrcpp::Any visitReference(iemlParser::ReferenceContext *ctx) override;

  /**
   *  REFERENCE VALUE
   */
  virtual antlrcpp::Any visitReference_value__identifier(iemlParser::Reference_value__identifierContext *ctx) override;
  virtual antlrcpp::Any visitReference_value__STRING(iemlParser::Reference_value__STRINGContext *ctx) override;
  virtual antlrcpp::Any visitReference_value__phrase(iemlParser::Reference_value__phraseContext *ctx) override;
};

}
