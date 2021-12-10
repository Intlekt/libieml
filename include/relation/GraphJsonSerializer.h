#pragma once

#include <memory>
#include <map>

#include "relation/BinaryRelation.h"
#include "structure/CategoryRegister.h"

#include <nlohmann/json.hpp>


namespace ieml::relation {

using namespace nlohmann;

template<class NodeType>
json serializeNode(const structure::CategoryRegister& categories, const NodeType& n, size_t id) {
    json names;
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
json graph_to_json(const std::shared_ptr<GraphType<NodeType, RelationType>> graph, const structure::CategoryRegister& categories) {
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
                    "path", rel->getAttributes()->getPath()->to_string()
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