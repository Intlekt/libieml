#pragma once

#include <nlohmann/json.hpp>
#include <enum.h>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "structure/path/PathTree.h"



namespace ieml::relation {

BETTER_ENUM(RelationType, char, COMPOSITION, INCLUSION, LINK)

class RelationAttribute {
public:
    virtual ~RelationAttribute() = default;

    virtual RelationType getRelationType() const = 0;

    virtual nlohmann::json to_json() const = 0;
};

struct Relation {
    Relation(std::shared_ptr<structure::Element> source_, 
             std::shared_ptr<structure::Element> target_, 
             std::shared_ptr<RelationAttribute> attribute_) : 
        source(source_), 
        target(target_), 
        attribute(attribute_) {}

    const std::shared_ptr<structure::Element> source; 
    const std::shared_ptr<structure::Element> target;
    const std::shared_ptr<RelationAttribute> attribute;

    // const CompositionAttributes cmp_attr_ = (CompositionAttributes){.path_=nullptr, .cmp_type_=CompositionRelationType::COMPOSITION_PHRASE};
};

class RelationGraph {
public:
    // RelationGraph() {}

    void add_node(const std::shared_ptr<structure::Element>& node);

    void add_relation(const Relation& relation);

    struct VertexProperties {
        VertexProperties(std::shared_ptr<structure::Element> element_) : element(element_) {}
        VertexProperties() : element(nullptr) {}

        const std::shared_ptr<structure::Element> element;
    };

    struct EdgeProperties {
        EdgeProperties(std::shared_ptr<RelationAttribute> attribute_) : attribute(attribute_) {}

        const std::shared_ptr<RelationAttribute> attribute;
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
