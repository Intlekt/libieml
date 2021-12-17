#pragma once

#include <nlohmann/json.hpp>
#include "IemlParser.h"
#include "SyntaxError.h"

#include <memory>
#include <map>

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
nlohmann::json serializeNode(const structure::CategoryRegister& categories, const NodeType& n, size_t id) {
    nlohmann::json names;
    auto name = categories.getName(n);
    for (auto it = name->begin(); it != name->end(); ++it)
        names[it->second.language()._to_string()] = it->second.value();
    

    return {
        {"id", id},
        {"class", categories.isNode(n) ? "NODE" : "COMPONENT"},
        {"names", names}
    };
}

template<template<class, class> class GraphType, class NodeType, class RelationType>
nlohmann::json binaryGraphToJson(const std::shared_ptr<GraphType<NodeType, RelationType>> graph, const structure::CategoryRegister& categories) {
    size_t id = 0;
    std::unordered_map<std::shared_ptr<NodeType>, size_t> node_to_idx;
    
    nlohmann::json nodes;
    nlohmann::json relations;
    for (auto it = graph->begin(); it != graph->end(); ++it) {
        for (auto it_r = it->second.begin(); it_r != it->second.end(); ++it_r) {
            auto rel = *it_r;
            size_t sbj_id, obj_id;

            auto sbj_it = node_to_idx.find(rel->getSubject());
            if (sbj_it == node_to_idx.end()) {
                sbj_id = id;
                node_to_idx.insert({rel->getSubject(), id});
                nodes.push_back(serializeNode(categories, rel->getSubject(), id));
                
                id++;
            } else sbj_id = sbj_it->second;

            auto obj_it = node_to_idx.find(rel->getObject());
            if (obj_it == node_to_idx.end()) {
                obj_id = id;
                node_to_idx.insert({rel->getObject(), id});
                nodes.push_back(serializeNode(categories, rel->getObject(), id));

                id++;
            } else obj_id = obj_it->second;


            relations.push_back({
                {"subject", sbj_id},
                {"object", obj_id},
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