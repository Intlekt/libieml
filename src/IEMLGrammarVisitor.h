#pragma once

#include <map>
#include <memory>

#include "iemlBaseVisitor.h"

#include "RecognitionError.h"

#include "ast/Declaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
#include "ast/Phrase.h"
#include "ast/InflexedCategory.h"


namespace ieml {
namespace parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlBaseVisitor {
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
  IEMLGrammarVisitor(IEMLParserErrorListener* error_listener) : iemlBaseVisitor(), error_listener_(error_listener) {}

  antlrcpp::Any visitDeclarations(iemlParser::DeclarationsContext *ctx) override {
    auto declarations = std::vector<std::unique_ptr<Declaration>>();

    for (auto children: ctx->declaration()) {
        declarations.emplace_back(std::move(visit(children).as<std::unique_ptr<Declaration>>()));
    }

    return std::make_unique<Program>(charRangeFromContext(ctx), std::move(declarations));
  }

  antlrcpp::Any visitComponent(iemlParser::ComponentContext *ctx) override {
    std::vector<std::unique_ptr<LanguageString>> translation_list;

    for (auto children : ctx->language_string()) {
      std::unique_ptr<LanguageString> language_string = std::move(visit(children).as<std::unique_ptr<LanguageString>>());
      if (language_string) {
        translation_list.emplace_back(std::move(language_string));
      }
    }

    std::unique_ptr<Phrase> phrase = std::move(visit(ctx->phrase()).as<std::unique_ptr<Phrase>>());

    return std::unique_ptr<Declaration>(new ComponentDeclaration(charRangeFromContext(ctx), std::move(translation_list), std::move(phrase)));
  }

  antlrcpp::Any visitLanguage_string(iemlParser::Language_stringContext *ctx) override {
    std::unique_ptr<Identifier> language = std::move(visitIdentifier(ctx->language).as<std::unique_ptr<Identifier>>());
    
    if( languageStr_to_LanguageType.count(language->getName()) < 1 ) {
      const std::string msg = "Invalid language identifier '" + language->getName() + "'";
      error_listener_->visitorError(language->getCharRange(), msg);

      return std::unique_ptr<LanguageString>(nullptr);  
    }

    LanguageType ltype = languageStr_to_LanguageType.at(language->getName());

    auto value = std::move(visitIdentifier(ctx->value).as<std::unique_ptr<Identifier>>());

    return std::make_unique<LanguageString>(charRangeFromContext(ctx), ltype, std::move(value));
  }

  antlrcpp::Any visitPhrase_lines(iemlParser::Phrase_linesContext *ctx) override {
    std::vector<std::unique_ptr<PhraseLine>> phrase_lines;

    for (auto child : ctx->phrase_line()) {
      auto phrase_line = std::move(visit(child).as<std::unique_ptr<PhraseLine>>());
      phrase_lines.emplace_back(std::move(phrase_line));
    }

    return std::unique_ptr<Phrase>(new SimplePhrase(charRangeFromContext(ctx), std::move(phrase_lines)));
  }

  
  antlrcpp::Any visitPhrase_line__sub_phrase_line_auxiliary(iemlParser::Phrase_line__sub_phrase_line_auxiliaryContext *ctx) override {
    int role_type = std::stoi(ctx->INTEGER()->getSymbol()->getText());

    if (role_type < 0 || role_type > 8) {
      error_listener_->visitorError(*charRangeFromToken(ctx->INTEGER()->getSymbol()), "Invalid role number, must in the [0,8] range.");
      return nullptr;
    }

    bool accentuation = ctx->accentuation;

    std::unique_ptr<AuxiliarySubPhraseLine> aux_line = std::move(visit(ctx->sub_phrase_line_auxiliary()).as<std::unique_ptr<AuxiliarySubPhraseLine>>());

    return std::unique_ptr<PhraseLine>(new SimplePhraseLine(charRangeFromContext(ctx), static_cast<RoleType>(role_type), accentuation, std::move(aux_line)));
  }

  antlrcpp::Any visitSub_phrase_line_auxiliary__sub_phrase_no_auxiliary(iemlParser::Sub_phrase_line_auxiliary__sub_phrase_no_auxiliaryContext *ctx) override {
    std::unique_ptr<Identifier> auxiliary = nullptr;
    if (ctx->auxiliary) {
      auxiliary = std::move(visit(ctx->auxiliary).as<std::unique_ptr<Identifier>>());
    }

    std::unique_ptr<InflexedCategory> inflexed_category = std::move(visit(ctx->inflexed_category()).as<std::unique_ptr<InflexedCategory>>());

    return std::unique_ptr<AuxiliarySubPhraseLine>(new SimpleAuxiliarySubPhraseLine(charRangeFromContext(ctx), std::move(auxiliary), std::move(inflexed_category)));
  }

  antlrcpp::Any visitInflexed_category(iemlParser::Inflexed_categoryContext *ctx) override {
    std::vector<std::unique_ptr<Identifier>> inflexions;

    for (auto & child : ctx->inflexions) {
      std::unique_ptr<Identifier> inflexion = std::move(visit(child).as<std::unique_ptr<Identifier>>());
      inflexions.emplace_back(std::move(inflexion)); 
    }
    
    std::unique_ptr<Identifier> category = std::move(visit(ctx->category).as<std::unique_ptr<Identifier>>());

    return std::make_unique<InflexedCategory>(charRangeFromContext(ctx), std::move(inflexions), std::move(category));
  }


  antlrcpp::Any visitIdentifier(iemlParser::IdentifierContext *ctx) override {
    std::ostringstream os;
    bool first = true;

    std::vector<antlr4::tree::TerminalNode *> identifiers = ctx->IDENTIFIER();

    for (auto* id_part : identifiers) {
      antlr4::Token* token = id_part->getSymbol();
      
      if (first)
        first = false;
      else
        os << " ";
      
      os << token->getText();
    }

    return std::make_unique<Identifier>(charRangeFromContext(ctx), os.str());
  }
};


}
}