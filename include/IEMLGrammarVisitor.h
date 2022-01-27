#pragma once

#include <string>
#include <map>
#include <memory>
#include <regex>

#include "iemlVisitor.h"
#include "SyntaxError.h"
#include "ast/CharRange.h"


namespace ieml::parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlVisitor {
private:
  const std::string file_id_;
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

  IEMLGrammarVisitor(const std::string& file_id, IEMLParserErrorListener* error_listener) 
      : iemlVisitor(), file_id_(file_id), error_listener_(error_listener) {}

  /**
   * PROGRAM
   */
  virtual antlrcpp::Any visitProgram(iemlParser::ProgramContext *ctx) override;

  /**
   * DECLARATION
   */
  virtual antlrcpp::Any visitComponentDeclaration(iemlParser::ComponentDeclarationContext *ctx) override;
  virtual antlrcpp::Any visitNodeDeclaration(iemlParser::NodeDeclarationContext *context) override;
  virtual antlrcpp::Any visitParanodeDeclaration(iemlParser::ParanodeDeclarationContext *context) override;
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
   * INFLEXION LIST
   */
  virtual antlrcpp::Any visitInflection_list__identifiers(iemlParser::Inflection_list__identifiersContext *ctx) override;
  virtual antlrcpp::Any visitInflection_list__words(iemlParser::Inflection_list__wordsContext *ctx) override;
  virtual antlrcpp::Any visitInflection_list_paradigm(iemlParser::Inflection_list_paradigmContext *ctx) override;

  /**
   * AUXILIARY
   */  
  virtual antlrcpp::Any visitAuxiliary__identifier(iemlParser::Auxiliary__identifierContext *context) override;
  virtual antlrcpp::Any visitAuxiliary__word(iemlParser::Auxiliary__wordContext *context) override;
  virtual antlrcpp::Any visitAuxiliary_paradigm(iemlParser::Auxiliary_paradigmContext *ctx) override;

  /**
   * AUXILIARY SIMPLE OR PARADIGM
   */  
  virtual antlrcpp::Any visitAuxiliary_simple_or_paradigm__simple(iemlParser::Auxiliary_simple_or_paradigm__simpleContext *ctx) override;
  virtual antlrcpp::Any visitAuxiliary_simple_or_paradigm__paradigm(iemlParser::Auxiliary_simple_or_paradigm__paradigmContext *ctx) override;

  /**
   * SUB PHRASELINE with AUXILIARY
   */
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__no_junction(iemlParser::Sub_phrase_line_auxiliary__no_junctionContext *ctx) override;
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__jonction(iemlParser::Sub_phrase_line_auxiliary__jonctionContext *ctx) override;

  /**
   * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
   */
  virtual antlrcpp::Any visitInflected_category__singular(iemlParser::Inflected_category__singularContext *context) override;
  virtual antlrcpp::Any visitInflected_category__category_paradigm(iemlParser::Inflected_category__category_paradigmContext *context) override;
  virtual antlrcpp::Any visitInflected_category__inflection_paradigm(iemlParser::Inflected_category__inflection_paradigmContext *context) override;
  virtual antlrcpp::Any visitInflected_category__inflection_and_category_paradigm(iemlParser::Inflected_category__inflection_and_category_paradigmContext *context) override;

  /**
   *  CATEGORY
   */
  virtual antlrcpp::Any visitCategory__identifier(iemlParser::Category__identifierContext *ctx) override;
  virtual antlrcpp::Any visitCategory__phrase(iemlParser::Category__phraseContext *ctx) override;
  virtual antlrcpp::Any visitCategory__word(iemlParser::Category__wordContext *ctx) override;
  virtual antlrcpp::Any visitCategory_paradigm(iemlParser::Category_paradigmContext *context) override;

  /**
   * WORD
   */
  virtual antlrcpp::Any visitWord(iemlParser::WordContext *context) override;

  /**
   *  LANGUAGE STRING
   */
  virtual antlrcpp::Any visitLanguage_string(iemlParser::Language_stringContext *ctx) override;

  /**
   *  IDENTIFIER
   */
  virtual antlrcpp::Any visitIdentifier(iemlParser::IdentifierContext *ctx) override;

  /**
   * JUNCTION
   */
  virtual antlrcpp::Any visitJunction(iemlParser::JunctionContext *context) override;

  /**
   * PATH
   */
  virtual antlrcpp::Any visitPath(iemlParser::PathContext *context) override;

  /**
   * PATH NODE
   */
  virtual antlrcpp::Any visitPath_node__root(iemlParser::Path_node__rootContext *context) override;
  virtual antlrcpp::Any visitPath_node__paradigm(iemlParser::Path_node__paradigmContext *context) override;
  virtual antlrcpp::Any visitPath_node__auxiliary(iemlParser::Path_node__auxiliaryContext *context) override;
  virtual antlrcpp::Any visitPath_node__inflection(iemlParser::Path_node__inflectionContext *context) override;
  virtual antlrcpp::Any visitPath_node__word(iemlParser::Path_node__wordContext *context) override;
  virtual antlrcpp::Any visitPath_node__junction(iemlParser::Path_node__junctionContext *context) override;
  virtual antlrcpp::Any visitPath_node__role_number(iemlParser::Path_node__role_numberContext *context) override;

  /**
   * DIMENSION DEFINITION
   */
  virtual antlrcpp::Any visitDimension_definition(iemlParser::Dimension_definitionContext *context) override;
  
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
