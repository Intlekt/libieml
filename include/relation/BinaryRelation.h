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


template<class NodeType, class RelationType>
class BinaryRelationGraph : public std::multimap<std::shared_ptr<NodeType>, std::shared_ptr<RelationType>> {
public:

    static std::shared_ptr<BinaryRelationGraph<NodeType, RelationType>> buildFromCategoryRegister(const structure::CategoryRegister& reg);

    void add_relation(const std::shared_ptr<RelationType>& relation) { this->insert({relation->getSubject(), relation}); }

    std::string to_string() const {
        std::stringstream os;

        for (auto it = this->begin(); it != this->end(); ++it) {
            os << it->second->to_string() << std::endl;
        }

        return os.str();
    }

};  

}}