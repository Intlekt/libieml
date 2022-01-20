#pragma once

#include <memory>
#include <map>

#include "ast/interfaces/AST.h"
#include "structure/path/PathTree.h"
#include "structure/Word.h"


namespace ieml {
namespace parser {

class SourceMapping {
public:
    SourceMapping() {};

    void register_mapping(std::shared_ptr<ieml::structure::PathTree> path_tree, ieml::AST::AST* declaration) {
        map_categories_.insert({
            path_tree,
            declaration
        });
    }

    const ieml::AST::AST* resolve_mapping(std::shared_ptr<ieml::structure::PathTree> path_tree) const {
        auto it = map_categories_.find(path_tree);
        if (it == map_categories_.end()) 
            return nullptr;
        return it->second;
    }

    void register_mapping(std::shared_ptr<ieml::structure::Word> word, ieml::AST::AST* declaration) {
        map_words_.insert({
            word,
            declaration
        });
    }

    const ieml::AST::AST* resolve_mapping(std::shared_ptr<ieml::structure::Word> word) const {
        auto it = map_words_.find(word);
        if (it == map_words_.end()) 
            return nullptr;
        return it->second;
    }

private:
    std::unordered_map<std::shared_ptr<ieml::structure::PathTree>, const ieml::AST::AST*> map_categories_;
    
    struct WordHasher {
        size_t operator()(const std::shared_ptr<ieml::structure::Word>& k) const {
            return std::hash<std::string>{}(k->getScript());
        }
    };
    struct WordEqual {
        size_t operator()(const std::shared_ptr<ieml::structure::Word>& l, const std::shared_ptr<ieml::structure::Word>& r) const {
            return l->getScript() == r->getScript();
        }
    };
    
    std::unordered_map<std::shared_ptr<ieml::structure::Word>, const ieml::AST::AST*, WordHasher, WordEqual> map_words_;
};

}}