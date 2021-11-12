#pragma once

#include <map>

#include "iemlBaseVisitor.h"
#include "ast/ComponentDeclaration.h"
#include "ast/interfaces/AST.h"
#include "ast/Constants.h"



namespace ieml {
namespace parser {

using namespace ieml_generated;
using namespace ieml::AST;


class IEMLGrammarVisitor: public iemlBaseVisitor {
public:
  virtual antlrcpp::Any visitDeclarations(iemlParser::DeclarationsContext *ctx) override {
    
    std::vector<antlrcpp::Any> declarations;

    for (auto children: ctx->declaration()) {
        declarations.push_back(visit(children));
    }

    return declarations;
  }

  virtual antlrcpp::Any visitComponent(iemlParser::ComponentContext *ctx) override {
    TranslationsMap t_map;

    for (auto lstr : ctx->language_string()) {
        auto pair = visit(lstr).as<std::pair<LanguageType, std::string>>();
        if (t_map.count(pair.first) > 0) {
            t_map[pair.first].push_back(pair.second);
        } else {
            TranslationList t_list;
            t_list.push_back(pair.second);
            t_map.insert(std::make_pair<>(pair.first, t_list));
        }
    }

    ComponentDeclaration cmp_decl(CharRange(1, 1, 1, 1), t_map);

    return cmp_decl;
  }

};

}
}