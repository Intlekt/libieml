#pragma once


#include <unordered_map>
#include <memory>
#include <vector>


namespace ieml {
namespace relation {

template<typename NodeType>
class BinaryRelation {
public:
    BinaryRelation(const std::shared_ptr<NodeType>& subject, const std::shared_ptr<NodeType>& object) : 
        subject_(subject), object_(object) {};

    const std::shared_ptr<NodeType>& getSubject() const {return subject_;};
    const std::shared_ptr<NodeType>& getObject() const {return object_;};

private:
    const std::shared_ptr<NodeType> subject_;
    const std::shared_ptr<NodeType> object_;
};


template<typename NodeType, typename RelationType>
class BinaryRelationGraph : public std::multimap<std::shared_ptr<NodeType>, std::shared_ptr<RelationType>> {
public:
    void add_relation(const std::shared_ptr<RelationType>& relation) { this->insert({relation->getSubject(), relation}); }
};  
}}