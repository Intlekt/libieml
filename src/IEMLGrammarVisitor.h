#pragma once

#include <map>
#include <memory>

#include "iemlBaseVisitor.h"
#include "ast/ComponentDeclaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"
#include "ast/Identifier.h"



namespace ieml {
namespace parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlBaseVisitor {
public:
  virtual antlrcpp::Any visitDeclarations(iemlParser::DeclarationsContext *ctx) override {
    
    auto declarations = std::make_unique<std::vector<std::unique_ptr<ComponentDeclaration>>>();

    for (auto children: ctx->declaration()) {
        declarations->push_back(std::move(visit(children).as<std::unique_ptr<ComponentDeclaration>>()));
    }

    return std::move(declarations);
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

    return std::make_unique<ComponentDeclaration>(CharRange(1, 1, 1, 1), t_map);
  }

  antlrcpp::Any visitFrench_language_string(iemlParser::French_language_stringContext *ctx) override {
    const Identifier id = visit(ctx->identifier()).as<Identifier>();

    return std::make_pair<>(LanguageType(FR), id);
  }

  antlrcpp::Any visitIdentifier(iemlParser::IdentifierContext *ctx) override {
    std::string name = "test";
    
    return Identifier(CharRange(1, 1, 1, 1), name);
  }
};


}
}