#pragma once

#include <memory>
#include <map>

#include "ast/interfaces/AST.h"
#include "structure/path/PathTree.h"
#include "structure/Word.h"
#include "structure/TableDefinition.h"


namespace ieml {
namespace parser {

/**
 * @brief SourceMapping stores the relationship between structure object and their AST declaration.
 * 
 *  The mapped types are :
 *      - ieml::structure::PathTree::Ptr
 *      - ieml::structure::Word::Ptr
 *      - ieml::structure::Table::Ptr
 * 
 */
class SourceMapping {
public:
    SourceMapping() = default;
    SourceMapping(const SourceMapping&) = delete;
    SourceMapping& operator=(SourceMapping&) = delete;

    /**
     * @brief Register the mapping between a PathTree pointer and its AST definition
     * 
     * @param path_tree 
     * @param declaration 
     */
    void register_mapping(const ieml::structure::PathTree::Ptr path_tree, const ieml::AST::AST* declaration) {
        map_categories_.insert({
            path_tree,
            declaration
        });
    }

    /**
     * @brief Get the AST object registered for this PathTree pointer. Return nullptr if the PathTree pointer is not registered.
     * 
     * @param path_tree 
     * @return const ieml::AST::AST* 
     */
    const ieml::AST::AST* resolve_mapping(const ieml::structure::PathTree::Ptr path_tree) const {
        auto it = map_categories_.find(path_tree);
        if (it == map_categories_.end()) 
            return nullptr;
        return it->second;
    }

    /**
     * @brief Register a mapping between a Word pointer and its AST definition.
     * 
     * @param word 
     * @param declaration 
     */
    void register_mapping(const ieml::structure::Word::Ptr& word, const ieml::AST::AST* declaration) {
        map_words_.insert({
            word,
            declaration
        });
    }

    /**
     * @brief Register a mapping between a Script pointer and its AST definition.
     * 
     * @param script 
     * @param declaration 
     */
    void register_mapping(const ieml::structure::Script::Ptr& script, const ieml::AST::AST* declaration) {
        map_scripts_.insert({
            script,
            declaration
        });
    }

    /**
     * @brief Get the AST object associated with this Word pointer. Return nullptr is the Word pointer is not registered.
     * 
     * @param word 
     * @return const ieml::AST::AST* 
     */
    const ieml::AST::AST* resolve_mapping(const ieml::structure::Word::Ptr& word) const {
        auto it = map_words_.find(word);
        if (it == map_words_.end()) 
            return nullptr;
        return it->second;
    }

    /**
     * @brief Register a mapping between a Table definition pointer and its AST definition.
     * 
     * @param table 
     * @param declaration 
     */
    void register_mapping(const ieml::structure::TableDefinition::Ptr& table, const ieml::AST::AST* declaration) {
        map_tables_.insert({
            table,
            declaration
        });
    }

    /**
     * @brief Get the AST object associated with this Table pointer. Return nullptr is the Table pointer is not registered.
     * 
     * @param table 
     * @return const ieml::AST::AST* 
     */
    const ieml::AST::AST* resolve_mapping(const ieml::structure::TableDefinition::Ptr& table) const {
        auto it = map_tables_.find(table);
        if (it == map_tables_.end()) 
            return nullptr;
        return it->second;
    }

    /**
     * @brief Get the AST object associated with this Script pointer. Return nullptr is the Script pointer is not registered.
     * 
     * @param script 
     * @return const ieml::AST::AST* 
     */
    const ieml::AST::AST* resolve_mapping(ieml::structure::Script::Ptr script) const {
        auto it = map_scripts_.find(script);
        if (it == map_scripts_.end()) 
            return nullptr;
        return it->second;
    }

private:
    struct WordHasher {
        size_t operator()(const ieml::structure::Word::Ptr& k) const {
            return std::hash<std::string>{}(k->getScript()->to_string());
        }
    };
    struct WordEqual {
        size_t operator()(const ieml::structure::Word::Ptr& l, const ieml::structure::Word::Ptr& r) const {
            return l->getScript() == r->getScript();
        }
    };
    
    std::unordered_map<ieml::structure::Word::Ptr, const ieml::AST::AST*, WordHasher, WordEqual> map_words_;
    std::unordered_map<ieml::structure::PathTree::Ptr, const ieml::AST::AST*> map_categories_;
    std::unordered_map<ieml::structure::TableDefinition::Ptr, const ieml::AST::AST*> map_tables_;
    std::unordered_map<ieml::structure::Script::Ptr, const ieml::AST::AST*> map_scripts_;
};
}}