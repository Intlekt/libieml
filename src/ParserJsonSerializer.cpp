#include "ParserJsonSerializer.h"

#include <functional>

using namespace ieml::parser;


nlohmann::json ieml::parser::charRangeToJson(const CharRange& char_range) {
    return {
        {"line_start", char_range.getLineStart()},
        {"line_end", char_range.getLineEnd()},
        {"char_start", char_range.getCharStart()},
        {"char_end", char_range.getCharEnd()}
    };
}

nlohmann::json ieml::parser::syntaxErrorToJson(const SyntaxError& syntax_error) {
    return {
        {"range", charRangeToJson(syntax_error.getCharRange())},
        {"message", syntax_error.getMessage()}
    };
}

nlohmann::json ieml::parser::nameToJson(const ieml::structure::Name& name) {
    nlohmann::json translations;

    for (auto it = name.begin(); it != name.end();) {
        
        auto key = it->first;
        nlohmann::json language_translations = nlohmann::json::array();

        while (it != name.end() && it->first == key) {
            language_translations.push_back(it->second.value());
            ++it;
        }

        translations[key._to_string()] = language_translations;
    }

    return translations;

}

nlohmann::json ieml::parser::errorManagerToJson(const IEMLParserErrorListener& error_manager) {
    nlohmann::json error_list = nlohmann::json::array();
    for (auto& error: error_manager.getSyntaxErrors()) {
        error_list.push_back(syntaxErrorToJson(*error));
    }
    return error_list;
}

nlohmann::json ieml::parser::conceptToJson(std::shared_ptr<ieml::structure::PathTree> concept, 
                                           ieml::parser::ParserContext& ctx,
                                           ieml::relation::CompositionRelationGraph& composition_graph) {
    auto hasher = std::hash<ieml::structure::PathTree>();
    
    size_t concept_id = hasher(*concept);
    const ieml::AST::CharRange& range = ctx.getSourceMapping().resolve_mapping(concept)->getCharRange();
    std::string node_type = (ctx.getCategoryRegister().isNode(concept) ? "NODE" : "COMPONENT");
    bool user_defined = true;

    std::string content;

    auto name = ctx.getCategoryRegister().getName(concept);

    nlohmann::json cited_concepts = nlohmann::json::array();
    nlohmann::json back_references = nlohmann::json::array();
    for (auto rel: composition_graph.getRelationsWithObject(concept)) {
        back_references.push_back(hasher(*rel->getSubject()));
    }

    return {
        {"concept_id", concept_id},
        {"range", charRangeToJson(range)},
        {"node_type", node_type},
        {"user_defined", user_defined},
        {"translations", nameToJson(*name)},
        {"definition", {
            {"content", content},
            {"cited_concepts", cited_concepts}
        }},
        {"back_references", back_references}
    };
}


nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    nlohmann::json j;
    j["errors"] = ieml::parser::errorManagerToJson(parser.getErrorListener());
    auto concepts = nlohmann::json::array();
    auto context = parser.getContext();
    auto cregister = context->getCategoryRegister();

    auto composition_graph = ieml::relation::CompositionRelationGraph::buildFromCategoryRegister(cregister);

    for (auto it = cregister.categories_begin(); it != cregister.categories_end(); ++it) {
        concepts.push_back(conceptToJson(it->first, *context, *composition_graph));
    }

    j["concepts"] = concepts;

    return j;
};
