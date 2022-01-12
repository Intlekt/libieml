#pragma once

#include <enum.h>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "structure/Path.h"



namespace ieml::relation {

BETTER_ENUM(RelationType, char, COMPOSITION, LINK);

BETTER_ENUM(CompositionRelationType, char, COMPOSITION_PHRASE, COMPOSITION_INFLECTION, COMPOSITION_AUXILIARY, COMPOSITION_JUNCTION, COMPOSITION_WORD);

struct CompositionAttributes {
    const std::shared_ptr<structure::Path> path_;
    const CompositionRelationType cmp_type_;
};
struct Relation {
    const std::shared_ptr<structure::Element> source_; 
    const std::shared_ptr<structure::Element> target_;

    const RelationType rel_type_;
    const CompositionAttributes cmp_attr_ = (CompositionAttributes){.path_=nullptr, .cmp_type_=CompositionRelationType::COMPOSITION_PHRASE};
    const std::shared_ptr<structure::PathTree> link_ = nullptr;
};

class RelationGraph {
public:
    // RelationGraph() {}

    void add_node(const std::shared_ptr<structure::Element>& node);

    void add_relation(const Relation& relation);

    struct VertexProperties {
        const std::shared_ptr<structure::Element> element;
    };

    struct EdgeProperties {
        const RelationType relation_type;
        const CompositionAttributes composition_attributes;
        const std::shared_ptr<structure::PathTree> link;
    };

    typedef boost::adjacency_list<boost::vecS, 
                                  boost::vecS, 
                                  boost::bidirectionalS, 
                                  VertexProperties, 
                                  EdgeProperties> Graph;

    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename boost::graph_traits<Graph>::edge_descriptor Edge;
    

    class Register {
    public:
        Vertex get_or_create(const std::shared_ptr<structure::Element>&, RelationGraph::Graph&);

    private:
        struct ElementHash_ {
            size_t operator()(const std::shared_ptr<structure::Element>& o) const {
                return o->hash();
            }
        };
        struct ElementEqual_ {
            bool operator()(const std::shared_ptr<structure::Element>& l, const std::shared_ptr<structure::Element>& r) const {
                return l == r;
            }
        };

        std::unordered_map<std::shared_ptr<structure::Element>, 
                           Vertex,
                           ElementHash_,
                           ElementEqual_> vertex_map_;
    };

    Register& getRegister() {return register_;};
    Graph& getGraph() {return graph_;};

private:

    Graph graph_;
    Register register_;
};
}
