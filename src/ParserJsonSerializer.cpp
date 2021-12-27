#include "ParserJsonSerializer.h"

#include "ast/interfaces/AST.h"

#include <functional>
#include <locale>


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
    std::locale loc;

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

std::string hashElement(std::shared_ptr<ieml::structure::PathTree> element) {
    auto hasher = std::hash<ieml::structure::PathTree>();
    return "category_" + std::to_string(hasher(*element));
}

std::string hashElement(std::shared_ptr<ieml::structure::Word> element) {
    auto hasher = std::hash<std::string>();
    return "word_" + std::to_string(hasher(element->to_string()));
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
        auto tgt = rel->getObject();
        if (tgt->isPathTree())
            references.push_back(hashElement(tgt->getPathTree()));
        else
            references.push_back(hashElement(tgt->getWord()));
    }

    nlohmann::json back_references = nlohmann::json::array();
    for (auto rel: composition_graph.getRelationsWithObject(node)) {
        auto tgt = rel->getSubject();
        if (tgt->isPathTree())
            back_references.push_back(hashElement(tgt->getPathTree()));
        else
            back_references.push_back(hashElement(tgt->getWord()));
    }

    return {
        {"id", hashElement(category)},
        {"range", charRangeToJson(range)},
        {"user_defined", user_defined},
        {"translations", nameToJson(*name)},
        {"type", "CATEGORY"},

        {"category_type", node_type},
        {"references", references},
        {"back_references", back_references}
    };
}


template <class WordType>
nlohmann::json _wordToJson(std::shared_ptr<WordType> word,
                           ieml::parser::ParserContext& ctx) {
    static_assert(std::is_base_of_v<ieml::structure::Word, WordType>, "WordType must derive from ieml::structure::Word");

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    const ieml::AST::CharRange& range = ast->getCharRange();

    bool user_defined = true;
    auto name = ctx.getWordRegister().getName(word);

    return {
        {"id", hashElement(word)},
        {"range", charRangeToJson(range)},
        {"user_defined", user_defined},
        {"translations", nameToJson(*name)},
        {"type", "WORD"},

        {"word_type", word->getWordType()._to_string()}
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
    
    nlohmann::json elements;

    for (auto it = cregister.categories_begin(); it != cregister.categories_end(); ++it)
        elements[hashElement(it->first)] = categoryToJson(it->first, *context, node_register, *composition_graph);
    
    for (auto it = wregister.auxiliaries_begin(); it != wregister.auxiliaries_end(); ++it)
        elements[hashElement(it->first)] = _wordToJson<ieml::structure::AuxiliaryWord>(it->first, *context);

    for (auto it = wregister.inflections_begin(); it != wregister.inflections_end(); ++it)
        elements[hashElement(it->first)] = _wordToJson<ieml::structure::InflectionWord>(it->first, *context);

    for (auto it = wregister.junctions_begin(); it != wregister.junctions_end(); ++it)
        elements[hashElement(it->first)] = _wordToJson<ieml::structure::JunctionWord>(it->first, *context);
  
    return {
        {"errors", errors},
        {"language", language._to_string()},
        {"elements", elements}
    };
};
