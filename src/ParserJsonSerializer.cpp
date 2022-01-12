#include "ParserJsonSerializer.h"

#include "ast/interfaces/AST.h"
#include "relation/Composition.h"
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
    
    for (auto l : structure::LanguageType::_values())
        translations[l._to_string()] = nlohmann::json::array();

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
                                            ieml::parser::ParserContextManager& ctx,
                                            ieml::relation::RelationGraph::Register& register_,
                                            ieml::relation::RelationGraph::Graph& graph) {
    
    auto vertex = register_.get_or_create(category, graph);

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(category));
    const ieml::AST::CharRange& range = ast->getCharRange();
    std::string node_type = (ctx.getCategoryRegister().isNode(category) ? "NODE" : "COMPONENT");
    bool user_defined = true;

    auto name = ctx.getCategoryRegister().getName(category);

    nlohmann::json references = nlohmann::json::array();
    typename boost::graph_traits<ieml::relation::RelationGraph::Graph>::out_edge_iterator out_i, out_end;
    for (std::tie(out_i, out_end) = boost::out_edges(vertex, graph); out_i != out_end; ++out_i) {
        auto tgt = boost::target(*out_i, graph);
        references.push_back(graph[tgt].element->uid());
    }

    nlohmann::json back_references = nlohmann::json::array();
    typename boost::graph_traits<ieml::relation::RelationGraph::Graph>::in_edge_iterator in_i, in_end;
    for (std::tie(in_i, in_end) = boost::in_edges(vertex, graph); in_i != in_end; ++in_i) {
        auto tgt = boost::source(*in_i, graph);
        references.push_back(graph[tgt].element->uid());
    }

    return {
        {"id", category->uid()},
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
                           ieml::parser::ParserContextManager& ctx) {
    static_assert(std::is_base_of_v<ieml::structure::Word, WordType>, "WordType must derive from ieml::structure::Word");

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    const ieml::AST::CharRange& range = ast->getCharRange();

    bool user_defined = true;
    auto name = ctx.getWordRegister().getName(word);

    return {
        {"id", word->uid()},
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

    ieml::relation::RelationGraph graph;
    ieml::relation::buildCompositionRelationGraph(graph, cregister, wregister);

    auto language = context->getLanguage();
    
    nlohmann::json elements = nlohmann::json::object();

    for (auto it = cregister.categories_begin(); it != cregister.categories_end(); ++it)
        elements[it->first->uid()] = categoryToJson(it->first, *context, graph.getRegister(), graph.getGraph());
    
    for (auto it = wregister.auxiliaries_begin(); it != wregister.auxiliaries_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::AuxiliaryWord>(it->first, *context);

    for (auto it = wregister.inflections_begin(); it != wregister.inflections_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::InflectionWord>(it->first, *context);

    for (auto it = wregister.junctions_begin(); it != wregister.junctions_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::JunctionWord>(it->first, *context);
  
    return {
        {"errors", errors},
        {"language", language._to_string()},
        {"elements", elements}
    };
};

nlohmann::json ieml::parser::serializeNode(const structure::CategoryRegister& categories, 
                             const structure::WordRegister& words,
                             const SourceMapping& mapping,
                             const std::shared_ptr<structure::Element>& n) {
    nlohmann::json names;

    std::shared_ptr<structure::Name> name;
    std::string ntype;
    std::string file_id;
    if (n->getElementType() == +structure::ElementType::PATH_TREE) {
        auto pt = std::dynamic_pointer_cast<structure::PathTree>(n);
        name = categories.getName(pt);
        ntype = categories.isNode(pt) ? "NODE" : "COMPONENT";
        file_id = mapping.resolve_mapping(pt)->getCharRange().getFileId();
    } else {
        auto w = std::dynamic_pointer_cast<structure::Word>(n);
        name = words.getName(w);
        ntype = w->getWordType()._to_string();
        file_id = mapping.resolve_mapping(w)->getCharRange().getFileId();
    }
    if (name)
        for (auto it = name->begin(); it != name->end(); ++it)
            names[it->second.language()._to_string()] = it->second.value();
    
    return { 
        {"id", n->uid()},
        {"class", ntype},
        {"names", names},
        {"file_id", file_id}
    };
};

nlohmann::json ieml::parser::binaryGraphToJson(ieml::relation::RelationGraph& relation_graph,
                                 const structure::CategoryRegister& categories,
                                 const structure::WordRegister& words,
                                 const SourceMapping& mapping) {
    size_t id = 0;
    std::unordered_set<ieml::relation::RelationGraph::Vertex> nodes_set;
    
    nlohmann::json nodes = nlohmann::json::array();
    auto& graph = relation_graph.getGraph();

    ieml::relation::RelationGraph::Graph::vertex_iterator v, vend;
    for (boost::tie(v, vend) = boost::vertices(graph); v != vend; ++v) {
        nodes_set.insert(*v);
        nodes.push_back(serializeNode(categories, words, mapping, graph[*v].element));
    }


    nlohmann::json relations = nlohmann::json::array();

    ieml::relation::RelationGraph::Graph::edge_iterator e, eend;
    for (boost::tie(e, eend) = boost::edges(graph); e != eend; ++e) {
        relations.push_back({
            {"subject", graph[boost::source(*e, graph)].element->uid()},
            {"object", graph[boost::target(*e, graph)].element->uid()},
            {"attributes", {
                {"type", std::string(graph[*e].relation_type._to_string())},
                {"composition_type", std::string(graph[*e].composition_attributes.cmp_type_._to_string())},
                {"path", graph[*e].composition_attributes.path_->to_string()}
            }}
        });

    }

    return {
        {"nodes", nodes},
        {"relations", relations}
    };
};
