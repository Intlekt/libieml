#pragma once


#include <unordered_map>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include <enum.h>
#include "structure/CategoryRegister.h"

namespace ieml {
namespace relation {


BETTER_ENUM(Class, char, COMPOSITION);


template<typename NodeType, typename AttributeType, class RelationAttributeTypeEnum>
class BinaryRelation {
public:
    BinaryRelation(const std::shared_ptr<NodeType>& subject, 
                   const std::shared_ptr<NodeType>& object,
                   const std::shared_ptr<AttributeType>& attributes,
                   const RelationAttributeTypeEnum type) : 
        subject_(subject), object_(object), attributes_(attributes), type_(type) {};

    const std::shared_ptr<NodeType>& getSubject() const {return subject_;};
    const std::shared_ptr<NodeType>& getObject() const {return object_;};
    const std::shared_ptr<AttributeType>& getAttributes() const {return attributes_;};
    RelationAttributeTypeEnum getType() const {return type_;};

    std::shared_ptr<BinaryRelation> concatenate(const std::shared_ptr<BinaryRelation<NodeType, AttributeType, RelationAttributeTypeEnum>>& other) {
        if (other == nullptr || object_ != other->subject_ || type_ != other->type_) 
            throw std::invalid_argument("Cannot concatenate two relations that are not connected or of different types.");

        return std::make_shared<BinaryRelation>(
            subject_,
            other->object_,
            attributes_->merge(other->attributes_),
            type_
        );
    };

    std::string to_string() const {
        return subject_->to_string() + " -> " + object_->to_string() + "[type=" + std::string(type_._to_string()) + " " + attributes_->to_string() + "]";
    }

private:
    const std::shared_ptr<NodeType> subject_;
    const std::shared_ptr<NodeType> object_;
    const std::shared_ptr<AttributeType> attributes_;
    const RelationAttributeTypeEnum type_;

};


template<class NodeType, class RelationType, class RelationAttributeTypeEnum>
class BinaryRelationGraph : public std::unordered_map<std::shared_ptr<NodeType>, std::unordered_set<std::shared_ptr<RelationType>>> {
public:
    void add_relation(const std::shared_ptr<RelationType>& relation) {
        this->operator[](relation->getSubject()).insert(relation);
        rev_relations_[relation->getObject()].insert(relation);

        vertexes_.insert(relation->getSubject());
        vertexes_.insert(relation->getObject());
    }

    std::shared_ptr<BinaryRelationGraph<NodeType, RelationType, RelationAttributeTypeEnum>> transitive_closure() {
        std::unordered_map<std::shared_ptr<NodeType>, size_t> vertex_to_id;
        std::unordered_map<size_t, std::shared_ptr<NodeType>> id_to_vertex;
        const size_t n_v = vertexes_.size();

        std::map<RelationAttributeTypeEnum, std::vector<std::vector<size_t>>> dist;
        std::map<RelationAttributeTypeEnum, std::vector<std::vector<size_t>>> next;
        for (auto rel_type: RelationAttributeTypeEnum::_values()) {
            dist[rel_type] = std::vector<std::vector<size_t>>(n_v);
            next[rel_type] = std::vector<std::vector<size_t>>(n_v);

            for (size_t i = 0; i < n_v; i++) {
                dist[rel_type][i] = std::vector<size_t>(n_v);
                next[rel_type][i] = std::vector<size_t>(n_v);
                for (size_t j = 0; j < n_v; j++) {
                    dist[rel_type][i][j] = n_v + 1;
                    next[rel_type][i][j] = n_v + 1;
                }
            }

        }
        size_t i = 0;
        for (auto it = vertexes_.cbegin(); it != vertexes_.cend(); ++it) {
            vertex_to_id[*it] = i;
            id_to_vertex[i] = *it;
            ++i;
        }

        // set dist and next to graph state
        for (auto it = this->cbegin(); it != this->cend(); ++it) {
            for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) {
                auto s = vertex_to_id[(*it_r)->getSubject()];
                auto o = vertex_to_id[(*it_r)->getObject()];
                next[(*it_r)->getType()][s][o] = o;
                dist[(*it_r)->getType()][s][o] = 1;
            }
        }
        for (auto rel_type: RelationAttributeTypeEnum::_values()) {
            for (size_t i = 0; i < n_v; i++) {
                next[rel_type][i][i] = i;
                dist[rel_type][i][i] = 0;
            }

            for (size_t k = 0; k < n_v; k++) {
                for (size_t i = 0; i < n_v; i++) {
                    for (size_t j = 0; j < n_v; j++) {
                        if (dist[rel_type][i][j] > dist[rel_type][i][k] + dist[rel_type][k][j]) {
                            dist[rel_type][i][j] = dist[rel_type][i][k] + dist[rel_type][k][j];
                            next[rel_type][i][j] = next[rel_type][i][k];
                        }
                    }
                }
            }
        }

        std::shared_ptr<BinaryRelationGraph<NodeType, RelationType, RelationAttributeTypeEnum>> graph = 
            std::make_shared<BinaryRelationGraph<NodeType, RelationType, RelationAttributeTypeEnum>>();
        
        for (auto rel_type: RelationAttributeTypeEnum::_values()) {
            for (size_t i = 0; i < n_v; i++) {
                for (size_t j = 0; j < n_v; j++) {
                    if (i != j && dist[rel_type][i][j] <= n_v) {
                        size_t k = i;
                        auto rel = getRelation(id_to_vertex[k], id_to_vertex[next[rel_type][k][j]]);
                        k = next[rel_type][k][j];
                        while (k != j) {
                            rel = rel->concatenate(getRelation(id_to_vertex[k], id_to_vertex[next[rel_type][k][j]]));
                            k = next[rel_type][k][j];
                        }
                        graph->add_relation(rel);
                    }
                }
            }
        }

        return graph;
    }

    std::string to_string() const {
        std::stringstream os;

        for (auto it = this->cbegin(); it != this->cend(); ++it) {
            for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) 
                os << (*it_r)->to_string() << std::endl;
        }

        return os.str();
    }

    std::shared_ptr<RelationType> getRelation(const std::shared_ptr<NodeType>& s, const std::shared_ptr<NodeType>& o) {
        auto it = this->find(s);
        if (it == this->end()) return nullptr;

        for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) {
            if ((*it_r)->getObject() == o) return (*it_r);
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<RelationType>> getRelationsWithObject(const std::shared_ptr<NodeType>& o) {
        std::vector<std::shared_ptr<RelationType>> res;
        auto it = rev_relations_.find(o);
        if (it == rev_relations_.end()) return res;

        for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) {
            res.push_back((*it_r));
        }
        return res;
    }
    std::vector<std::shared_ptr<RelationType>> getRelationsWithSubject(const std::shared_ptr<NodeType>& s) {
        std::vector<std::shared_ptr<RelationType>> res;
        auto it = this->find(s);
        if (it == this->end()) return res;

        for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) {
            res.push_back((*it_r));
        }
        return res;
    }

    size_t vertex_number() const {
        return vertexes_.size();
    }
    size_t relation_number() const {
        size_t count = 0;

        for (auto it = this->cbegin(); it != this->cend(); ++it) {
            for (auto it_r = it->second.cbegin(); it_r != it->second.cend(); ++it_r) 
                count += 1;
        }

        return count;
    }

private:
    std::set<std::shared_ptr<NodeType>> vertexes_;
    std::unordered_map<std::shared_ptr<NodeType>, std::unordered_set<std::shared_ptr<RelationType>>> rev_relations_;
};

}}