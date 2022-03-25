#pragma once


#include <string>
#include <vector>
#include <tuple>
#include <list>


#include "structure/path/PathTree.h"
#include "structure/reference/ReferenceType.h"
#include "structure/link/function/WordCondition.h"


namespace ieml::structure {
class ReferenceSchemaRegister;


class ReferenceSchema {
public:
    /**
     * A Reference Schema is :
     *  - a path tree
     *  - a list of Reference type
     *  - a list of list of path in the Reference Schema path tree
     *  - a list of string representing argument name (the signature). If the argument name is empty string, 
     *      indicates that the argument is not free (not in the link )
     */

    friend class ReferenceSchemaRegister;

    typedef std::tuple<
        PathTree::Ptr,                           // path_tree
        // std::vector<ReferenceType>,              // references types
        std::vector<std::vector<PathTree::Ptr>>, // arguments paths
        std::vector<std::string>                 // arguments names
    > Arguments;

    void define(std::vector<ReferenceValue> instance) {
        if (instance.size() != argument_names_.size())
            throw "Invalid instance arguments size";

        data_store_.push_back(instance);
    }

    const std::list<std::vector<ReferenceValue>>& getInstances() const {return data_store_;}

    WordCondition::VariableValuation valuation_from_reference_values(std::vector<ReferenceValue> references) const {
        WordCondition::VariableValuation res;
        for (size_t i = 0; i < argument_names_.size(); i++) {
            res.insert({argument_names_[i], references[i].getWord()});
        }
        return res;
    }

    std::vector<ReferenceValue> reference_values_from_valuation(WordCondition::VariableValuation valuation) const {
        std::vector<ReferenceValue> res;
        for (const auto& name: argument_names_) {
            res.push_back(ReferenceValue(ReferenceType::WORD, valuation.find(name)->second));
        }
        return res;
    }

private:

    ReferenceSchema(const Arguments& args) : 
        path_tree_(std::get<0>(args)),
        // references_type_(std::get<1>(args)),
        arguments_paths_(std::get<1>(args)),
        argument_names_(std::get<2>(args)) {}

    const PathTree::Ptr path_tree_;
    // const std::vector<ReferenceType> references_type_;
    const std::vector<std::vector<PathTree::Ptr>> arguments_paths_;
    const std::vector<std::string> argument_names_;


    // only path tree at the moment
    std::list<std::vector<ReferenceValue>> data_store_;
};


}