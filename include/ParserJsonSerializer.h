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
#include "SourceMapping.h"


namespace ieml::parser {

std::string hashElement(std::shared_ptr<ieml::structure::PathTree> element);
std::string hashElement(std::shared_ptr<ieml::structure::Word> element);

nlohmann::json charRangeToJson(const CharRange& char_range); 

nlohmann::json syntaxErrorToJson(const SyntaxError& syntax_error);

nlohmann::json nameToJson(const ieml::structure::Name& name);

nlohmann::json errorManagerToJson(const IEMLParserErrorListener& error_manager);

nlohmann::json categoryToJson(std::shared_ptr<ieml::structure::PathTree> concept, 
                             ieml::parser::ParserContextManager& ctx,
                             ieml::relation::CompositionNode::Register& node_register,
                             ieml::relation::CompositionRelationGraph& composition_graph);

nlohmann::json parserToJson(const IEMLParser& parser);

template<class NodeType>
nlohmann::json serializeNode(const structure::CategoryRegister& categories, 
                             const structure::WordRegister& words,
                             const SourceMapping& mapping,
                             const NodeType& n) {
    nlohmann::json names;

    std::shared_ptr<structure::Name> name;
    std::string ntype;
    std::string file_id;
    if (n->isPathTree()) {
        name = categories.getName(n->getPathTree());
        ntype = categories.isNode(n->getPathTree()) ? "NODE" : "COMPONENT";
        file_id = mapping.resolve_mapping(n->getPathTree())->getCharRange().getFileId();

    } else {
        name = words.getName(n->getWord());
        ntype = n->getWord()->getWordType()._to_string();
        file_id = mapping.resolve_mapping(n->getWord())->getCharRange().getFileId();
    }
    if (name)
        for (auto it = name->begin(); it != name->end(); ++it)
            names[it->second.language()._to_string()] = it->second.value();
    
    return { 
        {"id", n->getId()},
        {"class", ntype},
        {"names", names},
        {"file_id", file_id}
    };
}

template<template<class, class, class> class GraphType, class NodeType, class RelationType, class RelationAttributeTypeEnum>
nlohmann::json binaryGraphToJson(const std::shared_ptr<GraphType<NodeType, RelationType, RelationAttributeTypeEnum>> graph, 
                                 const structure::CategoryRegister& categories,
                                 const structure::WordRegister& words,
                                 const SourceMapping& mapping) {
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
                nodes.push_back(serializeNode(categories, words, mapping, rel->getSubject()));
            };

            auto obj_it = nodes_set.find(rel->getObject());
            if (obj_it == nodes_set.end()) {
                nodes_set.insert(rel->getObject());
                nodes.push_back(serializeNode(categories, words, mapping, rel->getObject()));
            };

            relations.push_back({
                {"subject", rel->getSubject()->getId()},
                {"object", rel->getObject()->getId()},
                {"attributes", {
                    {"type", std::string(rel->getType()._to_string())},
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