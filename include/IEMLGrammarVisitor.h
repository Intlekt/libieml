#pragma once


#include "IEMLParserGrammarVisitor.h"
#include "AbstractVisitor.h"


namespace ieml::parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public IEMLParserGrammarVisitor, public AbstractVisitor {  
public:
  IEMLGrammarVisitor(const std::string& file_id, IEMLParserErrorListener* error_listener) : 
      IEMLParserGrammarVisitor(), 
      AbstractVisitor(file_id, error_listener) {}

  /**
   * PROGRAM
   */
  virtual antlrcpp::Any visitProgram(IEMLParserGrammar::ProgramContext *ctx) override;

  /**
   * DECLARATION
   */
  virtual antlrcpp::Any visitComponentDeclaration(IEMLParserGrammar::ComponentDeclarationContext *ctx) override;
  virtual antlrcpp::Any visitNodeDeclaration(IEMLParserGrammar::NodeDeclarationContext *context) override;
  virtual antlrcpp::Any visitParanodeDeclaration(IEMLParserGrammar::ParanodeDeclarationContext *context) override;
  virtual antlrcpp::Any visitLinkDeclaration(IEMLParserGrammar::LinkDeclarationContext *context) override;
  virtual antlrcpp::Any visitWordDeclaration(IEMLParserGrammar::WordDeclarationContext *context) override;
  virtual antlrcpp::Any visitInflectionDeclaration(IEMLParserGrammar::InflectionDeclarationContext *context) override;
  virtual antlrcpp::Any visitAuxiliaryDeclaration(IEMLParserGrammar::AuxiliaryDeclarationContext *context) override;
  virtual antlrcpp::Any visitJunctionDeclaration(IEMLParserGrammar::JunctionDeclarationContext *context) override;
  virtual antlrcpp::Any visitLanguageDeclaration(IEMLParserGrammar::LanguageDeclarationContext *context) override;
  virtual antlrcpp::Any visitTableDeclaration(IEMLParserGrammar::TableDeclarationContext *context) override;
  virtual antlrcpp::Any visitRootParadigmDeclaration(IEMLParserGrammar::RootParadigmDeclarationContext *context) override;
  
  /**
   * PHRASE
   */
  virtual antlrcpp::Any visitPhrase__lines(IEMLParserGrammar::Phrase__linesContext *ctx) override;
  virtual antlrcpp::Any visitPhrase__junction(IEMLParserGrammar::Phrase__junctionContext *ctx) override;

  /**
   * PHRASELINE
   */
  virtual antlrcpp::Any visitPhrase_line__sub_phrase_line_auxiliary(IEMLParserGrammar::Phrase_line__sub_phrase_line_auxiliaryContext *ctx) override;
  virtual antlrcpp::Any visitPhrase_line__jonction_auxiliary(IEMLParserGrammar::Phrase_line__jonction_auxiliaryContext *ctx) override;

  /**
   * INFLEXION LIST
   */
  virtual antlrcpp::Any visitInflection_list__identifiers(IEMLParserGrammar::Inflection_list__identifiersContext *ctx) override;
  virtual antlrcpp::Any visitInflection_list__words(IEMLParserGrammar::Inflection_list__wordsContext *ctx) override;
  virtual antlrcpp::Any visitInflection_list_paradigm(IEMLParserGrammar::Inflection_list_paradigmContext *ctx) override;

  /**
   * AUXILIARY
   */  
  virtual antlrcpp::Any visitAuxiliary__identifier(IEMLParserGrammar::Auxiliary__identifierContext *context) override;
  virtual antlrcpp::Any visitAuxiliary__word(IEMLParserGrammar::Auxiliary__wordContext *context) override;
  virtual antlrcpp::Any visitAuxiliary_paradigm(IEMLParserGrammar::Auxiliary_paradigmContext *ctx) override;

  /**
   * AUXILIARY SIMPLE OR PARADIGM
   */  
  virtual antlrcpp::Any visitAuxiliary_simple_or_paradigm__simple(IEMLParserGrammar::Auxiliary_simple_or_paradigm__simpleContext *ctx) override;
  virtual antlrcpp::Any visitAuxiliary_simple_or_paradigm__paradigm(IEMLParserGrammar::Auxiliary_simple_or_paradigm__paradigmContext *ctx) override;

  /**
   * SUB PHRASELINE with AUXILIARY
   */
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__no_junction(IEMLParserGrammar::Sub_phrase_line_auxiliary__no_junctionContext *ctx) override;
  virtual antlrcpp::Any visitSub_phrase_line_auxiliary__jonction(IEMLParserGrammar::Sub_phrase_line_auxiliary__jonctionContext *ctx) override;

  /**
   * INFLEXED CATEGORY (SUB PHRASELINE without AUXILIARY)
   */
  virtual antlrcpp::Any visitInflected_category__singular(IEMLParserGrammar::Inflected_category__singularContext *context) override;
  virtual antlrcpp::Any visitInflected_category__category_paradigm(IEMLParserGrammar::Inflected_category__category_paradigmContext *context) override;
  virtual antlrcpp::Any visitInflected_category__inflection_paradigm(IEMLParserGrammar::Inflected_category__inflection_paradigmContext *context) override;
  virtual antlrcpp::Any visitInflected_category__inflection_and_category_paradigm(IEMLParserGrammar::Inflected_category__inflection_and_category_paradigmContext *context) override;

  /**
   *  CATEGORY
   */
  virtual antlrcpp::Any visitCategory__identifier(IEMLParserGrammar::Category__identifierContext *ctx) override;
  virtual antlrcpp::Any visitCategory__phrase(IEMLParserGrammar::Category__phraseContext *ctx) override;
  virtual antlrcpp::Any visitCategory__word(IEMLParserGrammar::Category__wordContext *ctx) override;
  virtual antlrcpp::Any visitCategory_paradigm(IEMLParserGrammar::Category_paradigmContext *context) override;

  /**
   *  INVARIANT TO PARANODE MAPPING
   */
  virtual antlrcpp::Any visitInvariant_paranode_mapping(IEMLParserGrammar::Invariant_paranode_mappingContext *context) override;

  /**
   * WORD
   */
  virtual antlrcpp::Any visitWord(IEMLParserGrammar::WordContext *context) override;

  /**
   *  LANGUAGE STRING
   */
  virtual antlrcpp::Any visitLanguage_string(IEMLParserGrammar::Language_stringContext *ctx) override;

  /**
   *  IDENTIFIER
   */
  virtual antlrcpp::Any visitIdentifier(IEMLParserGrammar::IdentifierContext *ctx) override;

  /**
   * JUNCTION
   */
  virtual antlrcpp::Any visitJunction__identifier(IEMLParserGrammar::Junction__identifierContext *context) override;
  virtual antlrcpp::Any visitJunction__word(IEMLParserGrammar::Junction__wordContext *context) override;

  /**
   * PATH
   */
  virtual antlrcpp::Any visitPath(IEMLParserGrammar::PathContext *context) override;

  /**
   * PATH NODE
   */
  virtual antlrcpp::Any visitPath_node__root(IEMLParserGrammar::Path_node__rootContext *context) override;
  virtual antlrcpp::Any visitPath_node__paradigm(IEMLParserGrammar::Path_node__paradigmContext *context) override;
  virtual antlrcpp::Any visitPath_node__auxiliary(IEMLParserGrammar::Path_node__auxiliaryContext *context) override;
  virtual antlrcpp::Any visitPath_node__inflection(IEMLParserGrammar::Path_node__inflectionContext *context) override;
  virtual antlrcpp::Any visitPath_node__word(IEMLParserGrammar::Path_node__wordContext *context) override;
  virtual antlrcpp::Any visitPath_node__junction(IEMLParserGrammar::Path_node__junctionContext *context) override;
  virtual antlrcpp::Any visitPath_node__role_number(IEMLParserGrammar::Path_node__role_numberContext *context) override;

  /**
   * DIMENSION DEFINITION
   */
  virtual antlrcpp::Any visitDimension_definition(IEMLParserGrammar::Dimension_definitionContext *context) override;
  
  /**
   * ROLE TYPE
   */
  virtual antlrcpp::Any visitRole_type__integer(IEMLParserGrammar::Role_type__integerContext *context) override;
  virtual antlrcpp::Any visitRole_type__identifier(IEMLParserGrammar::Role_type__identifierContext *context) override;

  /**
   * VARIABLE
   */
  virtual antlrcpp::Any visitVariable(IEMLParserGrammar::VariableContext *context) override;

  /**
   *  REFERENCE
   */
  virtual antlrcpp::Any visitReference(IEMLParserGrammar::ReferenceContext *ctx) override;

  /**
   *  REFERENCE VALUE
   */
  virtual antlrcpp::Any visitReference_value__identifier(IEMLParserGrammar::Reference_value__identifierContext *ctx) override;
  virtual antlrcpp::Any visitReference_value__STRING(IEMLParserGrammar::Reference_value__STRINGContext *ctx) override;
  virtual antlrcpp::Any visitReference_value__phrase(IEMLParserGrammar::Reference_value__phraseContext *ctx) override;
};

}
