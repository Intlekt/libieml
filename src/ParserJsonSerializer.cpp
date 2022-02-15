#include "ParserJsonSerializer.h"

#include "ast/interfaces/AST.h"
#include "relation/Composition.h"
#include "structure/Table.h"
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


nlohmann::json _paradigmLayoutToJson(const ieml::structure::ParadigmLayout& layout,
                                     const std::vector<size_t>& coords_prefix) {
    
    if (coords_prefix.size() == layout.getShape().size()) return layout.at(coords_prefix)->uid();

    size_t curr_dim = coords_prefix.size();

    nlohmann::json arr = nlohmann::json::array();
    for (size_t i = 0; i < layout.getShape()[curr_dim]; ++i) {
        std::vector<size_t> prefix(coords_prefix.begin(), coords_prefix.end());
        prefix.push_back(i);
        arr.push_back(_paradigmLayoutToJson(layout, prefix));
    }
    return arr;
}

nlohmann::json ieml::parser::categoryToJson(std::shared_ptr<ieml::structure::PathTree> category, 
                                            ieml::parser::ParserContextManager& ctx,
                                            ieml::relation::RelationGraph::Register& register_,
                                            ieml::relation::RelationGraph::Graph& graph) {
    
    auto vertex = register_.get_or_create(category, graph);

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(category));
    const ieml::AST::CharRange& range = ast->getCharRange();
    std::string node_type(ctx.getCategoryRegister().getDefinitionType(category)._to_string());
    // bool user_defined = true;

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

    nlohmann::json singular_sequences = nlohmann::json::array();
    for (const auto& ss: ieml::structure::PathTree::singular_sequences(category)) {
        singular_sequences.push_back(ss->uid());
    }

    const auto& preg = ctx.getParadigmRegister();

    nlohmann::json nDimension = 0;
    nlohmann::json invariant = category->uid();
    nlohmann::json layout = nullptr;
    if (category->is_paradigm()) {
        auto& reg = ctx.getPathTreeRegister();
        invariant = reg.buildFromPaths(reg.invariant_paths(category))->uid();
        const auto& playout = preg.get_layout(category);
        nDimension = playout.getShape().size();
        layout = _paradigmLayoutToJson(playout, {});
    }

    nlohmann::json paradigms = nlohmann::json::array();
    for (const auto& paradigm: preg.resolve_paradigms(category)) {
        paradigms.push_back(paradigm->uid());
    }

    return {
        {"id", category->uid()},
        {"range", charRangeToJson(range)},

        {"translations", nameToJson(*name)},
        {"type", "CATEGORY"},

        {"category_type", node_type},
        {"references", references},
        {"back_references", back_references},

        {"singular_sequences", singular_sequences},
        {"invariant", invariant},
        {"paradigms", paradigms},
        {"nDimension", nDimension},
        {"layout", layout}
    };
}


template <class WordType>
nlohmann::json _wordToJson(std::shared_ptr<WordType> word,
                           ieml::parser::ParserContextManager& ctx) {
    static_assert(std::is_base_of_v<ieml::structure::Word, WordType>, "WordType must derive from ieml::structure::Word");

    auto name = ctx.getWordRegister().getName(word);

    nlohmann::json name_json = nullptr;
    if (name)
        name_json = nameToJson(*name);
    
    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    const ieml::AST::CharRange& range = ast->getCharRange();

    return {
        {"id", word->uid()},
        {"range", charRangeToJson(range)},
        {"translations", name_json},
        {"type", "WORD"},
        {"word_type", word->getWordType()._to_string()}
    };
}

nlohmann::json ieml::parser::serializeTable(const ieml::structure::Table& table) {
    // auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    // const ieml::AST::CharRange& range = ast->getCharRange();

    

    auto invariant_mapping = nlohmann::json::object();

    for (auto& invariant_pair: table.getInvariantMapping()) {
        invariant_mapping[invariant_pair.first->uid()] = invariant_pair.second->uid();
    }

    return {
        // {"range", charRangeToJson(range)},
        {"root", table.getRoot()->uid()},
        {"invariant_mapping", invariant_mapping}
    };
}

nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    auto errors = ieml::parser::errorManagerToJson(parser.getErrorListener());
    auto context = parser.getContext();
    auto cregister = context->getCategoryRegister();
    auto wregister = context->getWordRegister();

    ieml::relation::RelationGraph graph;
    ieml::relation::buildCompositionRelationGraph(graph, context->getPathTreeRegister(), cregister, wregister);

    auto language = context->getLanguage();
    
    nlohmann::json elements = nlohmann::json::object();

    for (auto it = cregister.categories_begin(); it != cregister.categories_end(); ++it)
        elements[it->first->uid()] = categoryToJson(it->first, *context, graph.getRegister(), graph.getGraph());
    
    for (auto it = wregister.category_word_begin(); it != wregister.category_word_end(); ++it)
        elements[it->second->uid()] = _wordToJson<ieml::structure::CategoryWord>(it->second, *context);

    for (auto it = wregister.auxiliaries_begin(); it != wregister.auxiliaries_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::AuxiliaryWord>(it->first, *context);

    for (auto it = wregister.inflections_begin(); it != wregister.inflections_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::InflectionWord>(it->first, *context);

    for (auto it = wregister.junctions_begin(); it != wregister.junctions_end(); ++it)
        elements[it->first->uid()] = _wordToJson<ieml::structure::JunctionWord>(it->first, *context);
  
    nlohmann::json tables = nlohmann::json::array();
    for (auto table: context->getParadigmRegister().getTables()) {
        tables.push_back(serializeTable(table));
    }

    return {
        {"errors", errors},
        {"language", language._to_string()},
        {"elements", elements},
        {"tables", tables}
    };
}

nlohmann::json ieml::parser::serializeNode(const structure::CategoryRegister& categories, 
                             const structure::WordRegister& words,
                             const SourceMapping& mapping,
                             const std::shared_ptr<structure::Element>& n) {

    std::shared_ptr<structure::Name> name;
    std::string ntype;
    nlohmann::json char_range;
    if (n->getElementType() == +structure::ElementType::PATH_TREE) {
        auto pt = std::dynamic_pointer_cast<structure::PathTree>(n);
        name = categories.getName(pt);
        ntype = categories.isNode(pt) ? "NODE" : "COMPONENT";
        char_range = charRangeToJson(mapping.resolve_mapping(pt)->getCharRange());
    } else {
        auto w = std::dynamic_pointer_cast<structure::Word>(n);
        name = words.getName(w);
        ntype = w->getWordType()._to_string();
        char_range = charRangeToJson(mapping.resolve_mapping(w)->getCharRange());
    }

    nlohmann::json names;

    if (name)
        names = nameToJson(*name);
    
    return { 
        {"id", n->uid()},
        {"class", ntype},
        {"names", names},
        {"range", char_range}
    };
}

nlohmann::json ieml::parser::binaryGraphToJson(ieml::relation::RelationGraph& relation_graph,
                                               const structure::CategoryRegister& categories,
                                               const structure::WordRegister& words,
                                               const SourceMapping& mapping) {
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
        const auto attribute = graph[*e].attribute;

        relations.push_back({
            {"subject", graph[boost::source(*e, graph)].element->uid()},
            {"object", graph[boost::target(*e, graph)].element->uid()},
            {"attributes", attribute->to_json()}
        });

    }

    return {
        {"nodes", nodes},
        {"relations", relations}
    };
}
