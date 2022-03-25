#include "ast/declaration/TableDeclaration.h"
#include "structure/TableDefinition.h"


using namespace ieml::AST;


std::string TableDeclaration::to_string() const {
    return "";
}


void TableDeclaration::check_declaration(ieml::parser::ParserContextManager& ctx) const {
    // the paradigm at the top of the paradigm hierarchy
    ieml::structure::PathTree::Ptr root;

    // the mapping between singular sequences of root or one of its descendent paradigms to other paradigmatic path trees.
    ieml::structure::TableDefinition::InvariantMapping invariant_to_paradigm;
    
    // the currently opened singular sequences leafs of the paradigm hierarchy
    std::unordered_set<ieml::structure::PathTree::Ptr> accepted_invariant;
    
    bool failed = false;

    for (auto&& map : mappings_) {
        auto pathtree_pair = map->check_mapping(ctx);
        if (!pathtree_pair) {
            failed = true;
            continue;
        }

        auto invariant = pathtree_pair->first;
        auto paranode = pathtree_pair->second; 

        
        // no repeated invariant
        auto it = invariant_to_paradigm.find(invariant);
        if (it != invariant_to_paradigm.end()) {
            ctx.getErrorManager().visitorError(
                map->getCharRange(), 
                "The invariant has already been defined."
            );
            failed = true;
            continue;
        }

        // invariant is defined in the same root
        // define root
        if (accepted_invariant.size() == 0) {
            root = invariant;
        } else {
            auto it = accepted_invariant.find(invariant);
            if (it == accepted_invariant.end()) {
                ctx.getErrorManager().visitorError(
                    map->getCharRange(), 
                    "This invariant is not a singular sequence of previous paranodes in this table declaration."
                );
                failed = true;
                continue;
            }
        }

        auto singular_sequences = ieml::structure::PathTree::singular_sequences(paranode);
        accepted_invariant.insert(singular_sequences.begin(), singular_sequences.end());

        invariant_to_paradigm.insert({invariant, paranode});
    }
    if (failed)
        return;

    // TODO : ensure we cannot instensiate multiple times the same table ? 
    //          do we want unicity like the PathTrees ?
    const auto table = std::make_shared<ieml::structure::TableDefinition>(root, invariant_to_paradigm);

    ctx.getParadigmRegister().register_table(table);
    ctx.getSourceMapping().register_mapping(table, this);

    return;
}
