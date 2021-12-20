#pragma once

#include <nlohmann/json.hpp>
#include "IemlParser.h"
#include "SyntaxError.h"

#include <memory>
#include <set>

#include "relation/BinaryRelation.h"
#include "relation/Composition.h"
#include "structure/CategoryRegister.h"
#include "structure/Path.h"
#include "structure/Namespace.h"


namespace ieml::parser {

nlohmann::json charRangeToJson(const CharRange& char_range); 

nlohmann::json syntaxErrorToJson(const SyntaxError& syntax_error);

nlohmann::json nameToJson(const ieml::structure::Name& name);

nlohmann::json errorManagerToJson(const IEMLParserErrorListener& error_manager);

nlohmann::json categoryToJson(std::shared_ptr<ieml::structure::PathTree> concept, 
                             ieml::parser::ParserContext& ctx,
                             ieml::relation::CompositionNode::Register& node_register,
                             ieml::relation::CompositionRelationGraph& composition_graph);

nlohmann::json parserToJson(const IEMLParser& parser);

template<class NodeType>
nlohmann::json serializeNode(const structure::CategoryRegister& categories, 
                             const structure::WordRegister& words, 
                             const NodeType& n) {
    nlohmann::json names;

    std::shared_ptr<structure::Name> name;
    std::string ntype;
    if (n->isPathTree()) {
        name = categories.getName(n->getPathTree());
        ntype = categories.isNode(n->getPathTree()) ? "NODE" : "COMPONENT";
    } else {
        name = words.getName(n->getWord());
        ntype = n->getWord()->getWordType()._to_string();
    }
    for (auto it = name->begin(); it != name->end(); ++it)
        names[it->second.language()._to_string()] = it->second.value();
    

    return {
        {"id", n->getId()},
        {"class", ntype},
        {"names", names}
    };
}

template<template<class, class> class GraphType, class NodeType, class RelationType>
nlohmann::json binaryGraphToJson(const std::shared_ptr<GraphType<NodeType, RelationType>> graph, 
                                 const structure::CategoryRegister& categories,
                                 const structure::WordRegister& words) {
    size_t id = 0;
    std::unordered_set<std::shared_ptr<NodeType>> nodes_set;
    
    nlohmann::json nodes;
    nlohmann::json relations;
    for (auto it = graph->begin(); it != graph->end(); ++it) {
        for (auto it_r = it->second.begin(); it_r != it->second.end(); ++it_r) {
            auto rel = *it_r;

            auto sbj_it = nodes_set.find(rel->getSubject());
            if (sbj_it == nodes_set.end()) {
                nodes_set.insert(rel->getSubject());
                nodes.push_back(serializeNode(categories, words, rel->getSubject()));
            };

            auto obj_it = nodes_set.find(rel->getObject());
            if (obj_it == nodes_set.end()) {
                nodes_set.insert(rel->getObject());
                nodes.push_back(serializeNode(categories, words, rel->getObject()));
            };

            relations.push_back({
                {"subject", rel->getSubject()->getId()},
                {"object", rel->getObject()->getId()},
                {"attributes", {
                    {"path", rel->getAttributes()->getPath()->to_string()}
                }}
            });
        }
    }

    return {
        {"nodes", nodes},
        {"relations", relations}
    };
};

}