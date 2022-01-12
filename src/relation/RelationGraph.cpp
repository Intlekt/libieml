#include "relation/RelationGraph.h"

#include <string>

using namespace ieml::relation;


RelationGraph::Vertex RelationGraph::Register::get_or_create(const std::shared_ptr<structure::Element>& element, 
                                                             RelationGraph::Graph& graph) {
    auto it = vertex_map_.find(element);
    if (it != vertex_map_.end())
        return it->second;

    const Vertex& vertex = add_vertex(
        (VertexProperties){.element=element},
        graph
    );
    vertex_map_.insert({element, vertex});
    return vertex;
}


void RelationGraph::add_node(const std::shared_ptr<structure::Element>& element) {
    register_.get_or_create(element, graph_);
}

void RelationGraph::add_relation(const Relation& relation) {
    const Vertex& source = register_.get_or_create(relation.source_, graph_);
    const Vertex& target = register_.get_or_create(relation.target_, graph_);
    
    add_edge(source, 
             target, 
             (EdgeProperties){
                 .relation_type = relation.rel_type_,
                 .composition_attributes = relation.cmp_attr_,
                 .link = relation.link_
             }, 
             graph_);
}