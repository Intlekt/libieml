#include "ParserJsonSerializer.h"

#include "ast/interfaces/AST.h"

#include <functional>

using namespace ieml::parser;


nlohmann::json ieml::parser::charRangeToJson(const CharRange& char_range) {
    return {
        {"file_id", char_range.getFileId()},
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

nlohmann::json ieml::parser::categoryToJson(std::shared_ptr<ieml::structure::PathTree> category, 
                                           ieml::parser::ParserContext& ctx,
                                           ieml::relation::CompositionNode::Register& node_register,
                                           ieml::relation::CompositionRelationGraph& composition_graph) {
    
    auto node = node_register.get_or_create(category);

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(category));
    const ieml::AST::CharRange& range = ast->getCharRange();
    std::string node_type = (ctx.getCategoryRegister().isNode(category) ? "NODE" : "COMPONENT");
    bool user_defined = true;

    auto name = ctx.getCategoryRegister().getName(category);

    nlohmann::json references = nlohmann::json::array();
    for (auto rel: composition_graph.getRelationsWithSubject(node)) {
        references.push_back(rel->getObject()->getId());
    }

    nlohmann::json back_references = nlohmann::json::array();
    for (auto rel: composition_graph.getRelationsWithObject(node)) {
        back_references.push_back(rel->getSubject()->getId());
    }

    return {
        {"category_id", node->getId()},
        {"range", charRangeToJson(range)},
        {"node_type", node_type},
        {"user_defined", user_defined},
        {"translations", nameToJson(*name)},
        {"references", references},
        {"back_references", back_references}
    };
}

template <class WordType>
nlohmann::json _wordToJson(std::shared_ptr<WordType> word,
                           ieml::parser::ParserContext& ctx) {
    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    const ieml::AST::CharRange& range = ast->getCharRange();

    bool user_defined = true;
    auto name = ctx.getWordRegister().getName(word);

    return {
        {"word_id", std::hash<WordType>()(*word)},
        {"range", charRangeToJson(range)},
        {"user_defined", user_defined},
        {"translations", nameToJson(*name)}
    };
}

nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    auto errors = ieml::parser::errorManagerToJson(parser.getErrorListener());
    auto context = parser.getContext();
    auto cregister = context->getCategoryRegister();
    auto wregister = context->getWordRegister();
    auto node_register = ieml::relation::CompositionNode::Register();

    auto composition_graph = ieml::relation::buildCompositionRelationGraph(node_register, cregister, wregister);

    auto language = context->getLanguage();
    
    nlohmann::json namespace_category, namespace_auxiliary, namespace_inflection, namespace_junction;
    nlohmann::json categories = nlohmann::json();
    nlohmann::json auxiliaries = nlohmann::json();
    nlohmann::json inflections = nlohmann::json();
    nlohmann::json junctions = nlohmann::json();

    auto chasher = std::hash<ieml::structure::PathTree>();
    for (auto it = cregister.categories_begin(); it != cregister.categories_end(); ++it) {
        categories.push_back(categoryToJson(it->first, *context, node_register, *composition_graph));

        auto range = it->second->equal_range(language);

        for (auto it_n = range.first; it_n != range.second; ++it_n)
            namespace_category[it_n->second.value()] = chasher(*it->first);
    }
    
    auto aux_hasher = std::hash<ieml::structure::AuxiliaryWord>();
    for (auto it = wregister.auxiliaries_begin(); it != wregister.auxiliaries_end(); ++it) {
        auxiliaries.push_back(_wordToJson<ieml::structure::AuxiliaryWord>(it->first, *context));

        auto range = it->second->equal_range(language);
        for (auto it_n = range.first; it_n != range.second; ++it_n)
            namespace_auxiliary[it_n->second.value()] = aux_hasher(*it->first);
    }

    auto infl_hasher = std::hash<ieml::structure::InflectionWord>();
    for (auto it = wregister.inflections_begin(); it != wregister.inflections_end(); ++it) {
        inflections.push_back(_wordToJson<ieml::structure::InflectionWord>(it->first, *context));

        auto range = it->second->equal_range(language);
        for (auto it_n = range.first; it_n != range.second; ++it_n)
            namespace_inflection[it_n->second.value()] = infl_hasher(*it->first);
    }

    auto jct_hasher = std::hash<ieml::structure::JunctionWord>();
    for (auto it = wregister.junctions_begin(); it != wregister.junctions_end(); ++it) {
        junctions.push_back(_wordToJson<ieml::structure::JunctionWord>(it->first, *context));

        auto range = it->second->equal_range(language);
        for (auto it_n = range.first; it_n != range.second; ++it_n)
            namespace_junction[it_n->second.value()] = jct_hasher(*it->first);
    }

    return {
        {"errors", errors},
        {"language", language._to_string()},
        {"categories", categories},
        {"namespace_category", namespace_category},
        {"auxiliaries", auxiliaries},
        {"namespace_auxiliary", namespace_auxiliary},
        {"inflections", inflections},
        {"namespace_inflection", namespace_inflection},
        {"junctions", junctions},
        {"namespace_junction", namespace_junction}
    };
};
