#pragma once

#include <map>
#include <memory>

#include "iemlBaseVisitor.h"
#include "ast/Declaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"
#include "ast/Phrase.h"


namespace ieml {
namespace parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlBaseVisitor {
private:
  IEMLParserErrorListener* error_listener_;


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

  virtual antlrcpp::Any visitDeclarations(iemlParser::DeclarationsContext *ctx) override {
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

    return std::unique_ptr<Declaration>(new ComponentDeclaration(charRangeFromContext(ctx), std::move(translation_list)));
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