#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


namespace ieml::structure {
class LinkArgumentDefinition {
public:
    LinkArgumentDefinition(std::vector<std::string> variables) :
        variables_(variables),
        variable_to_index_(variable_index(variables)) {}

    std::unordered_set<std::string> getVariableNameSet() const {
        return std::unordered_set<std::string>(variables_.begin(), variables_.end());
    }

private:
    const std::vector<std::string> variables_;
    const std::unordered_map<std::string, size_t> variable_to_index_;

    static std::unordered_map<std::string, size_t> variable_index(std::vector<std::string> variables) {
        std::unordered_map<std::string, size_t> res;
        for (size_t i = 0; i < variables.size(); i++)
            res.insert({variables[i], i});
        return res;
    }
};
}