#pragma once

#include <map>
#include <memory>

#include "iemlBaseVisitor.h"
#include "ast/Declaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"
#include "ast/Program.h"


namespace ieml {
namespace parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlBaseVisitor {
private:
  IEMLParserErrorListener* error_listener_;

public:
  IEMLGrammarVisitor(IEMLParserErrorListener* error_listener) : iemlBaseVisitor(), error_listener_(error_listener) {}

  virtual antlrcpp::Any visitDeclarations(iemlParser::DeclarationsContext *ctx) override {
    
    auto declarations = std::vector<std::unique_ptr<Declaration>>();

    for (auto children: ctx->declaration()) {
        declarations.emplace_back(std::move(visit(children).as<std::unique_ptr<Declaration>>()));
    }

    return std::make_unique<Program>(CharRange(1, 1, 1, 1), std::move(declarations));
  }

  antlrcpp::Any visitComponent(iemlParser::ComponentContext *ctx) override {
    TranslationsMap t_map;

    for (auto lstr : ctx->language_string()) {
        auto pair = visit(lstr).as<std::pair<LanguageType, Identifier>>();
        if (t_map.count(pair.first) > 0) {
            t_map[pair.first].push_back(pair.second);
        } else {
            TranslationList t_list;
            t_list.push_back(pair.second);
            t_map.insert(std::make_pair<>(pair.first, t_list));
        }
    }

    return std::unique_ptr<Declaration>(new ComponentDeclaration(CharRange(1, 1, 1, 1), t_map));
  }

  antlrcpp::Any visitLanguage_string(iemlParser::Language_stringContext *ctx) override {
    Identifier language = visitIdentifier(ctx->language).as<Identifier>();
    
    if( languageStr_to_LanguageType.count(language.getName()) < 1 ) {
      const std::string msg = "Invalid language identifier '" + language.getName() + "'";
      error_listener_->visitorError(language.getCharRange(), msg);
    }

    LanguageType ltype = languageStr_to_LanguageType.at(language.getName());

    Identifier value = visitIdentifier(ctx->value).as<Identifier>();

    return std::make_pair<>(ltype, value);
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
    antlr4::Token* start_token = identifiers[0]->getSymbol();
    size_t l_start = start_token->getLine();
    size_t c_start = start_token->getCharPositionInLine();

    antlr4::Token* end_token = identifiers[identifiers.size() - 1]->getSymbol();
    size_t l_end = end_token->getLine();
    size_t c_end = end_token->getCharPositionInLine() + end_token->getText().size();

    return Identifier(CharRange(l_start, l_end, c_start, c_end), os.str());
  }
};


}
}