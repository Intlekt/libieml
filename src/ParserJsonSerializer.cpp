#include "ParserJsonSerializer.h"

#include "ast/interfaces/AST.h"
#include "relation/Composition.h"
#include "structure/TableDefinition.h"
#include <functional>
#include <locale>

#include <tuple>


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

std::pair<nlohmann::json, nlohmann::json> ieml::parser::errorManagerToJson(const IEMLParserErrorListener& error_manager) {
    nlohmann::json error_list = nlohmann::json::array();
    for (auto& error: error_manager.getSyntaxErrors()) {
        error_list.push_back(syntaxErrorToJson(*error));
    }

    nlohmann::json warning_list = nlohmann::json::array();
    for (auto& warning: error_manager.getSyntaxWarnings()) {
        warning_list.push_back(syntaxErrorToJson(*warning));
    }

    return {error_list, warning_list};
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

    nlohmann::json invariant = category->uid();
    nlohmann::json table;
    if (category->is_paradigm()) {
        auto& reg = ctx.getPathTreeRegister();
        invariant = reg.buildFromPaths(reg.invariant_paths(category))->uid();
        const auto& tablend = preg.get_table(category);
        table = tablend->uid();
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

        {"table", table}
    };
}


template <class WordType>
nlohmann::json _wordToJson(std::shared_ptr<WordType> word,
                           ieml::parser::ParserContextManager& ctx) {
    static_assert(std::is_base_of_v<ieml::structure::Word, WordType>, "WordType must derive from ieml::structure::Word");

    auto name = ctx.getWordRegister().getName(word);

    nlohmann::json name_json;
    if (name)
        name_json = nameToJson(*name);
    else
        name_json = nameToJson(ieml::structure::Name({}));
    
    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(word));
    const ieml::AST::CharRange& range = ast->getCharRange();

    return {
        {"id", word->uid()},
        {"range", charRangeToJson(range)},
        {"translations", name_json},
        {"type", "WORD"},
        {"word_type", word->getWordType()._to_string()},
        {"declaration", word->getScript()->uid()}
    };
}

nlohmann::json _scriptToJson(ieml::structure::Script::Ptr script, 
                             ieml::parser::ParserContextManager& ctx, 
                             const ieml::AST::CharRange& default_range) {

    nlohmann::json singular_sequences = nlohmann::json::array();
    for (const auto& ss : script->singular_sequences())
        singular_sequences.push_back(ss->uid());
    
    const auto type = script->get_type();
    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(script));
    nlohmann::json range_json;
    if (ast) range_json = charRangeToJson(ast->getCharRange());
    else     range_json = charRangeToJson(default_range);

    const auto name = ieml::structure::Name({});

    const auto word = ctx.getWordRegister().get_word_from_script(script);
    nlohmann::json word_json;
    if (word) word_json = word->uid();

    nlohmann::json word_type;
    if (ctx.getWordRegister().script_is_declared(script))   
        word_type = ctx.getWordRegister().get_script_type(script)._to_string();

    nlohmann::json res = {
        {"id", script->uid()},
        {"range", range_json},
        {"translations", nameToJson(name)},
        {"type", "SCRIPT"},

        {"str", script->to_string()},
        {"layer", script->get_layer()},
        {"multiplicity", script->get_multiplicity()},
        {"singular_sequences", singular_sequences},
        {"script_type", type._to_string()},

        // if defined, point to the word
        {"definition", word_json},
        {"declaration_type", word_type},

        {"table", ctx.getScriptRegister().get_or_create_table(script)->uid()}
    };

    // TODO : add the relation of this script to other scripts

    return res;
}

nlohmann::json ieml::parser::serializeCategoryHierarchy(ieml::parser::ParserContextManager& ctx,
                                            const ieml::structure::TableDefinition::Ptr& table) {

    auto ast = dynamic_cast<const ieml::AST::AST*>(ctx.getSourceMapping().resolve_mapping(table));
    const ieml::AST::CharRange& range = ast->getCharRange();

    auto invariant_mapping = nlohmann::json::object();

    for (auto& invariant_pair: table->getInvariantMapping()) {
        invariant_mapping[invariant_pair.first->uid()] = invariant_pair.second->uid();
    }

    return {
        {"range", charRangeToJson(range)},
        {"root", table->getRoot()->uid()},
        {"invariant_mapping", invariant_mapping}
    };
}

nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    auto errors_and_warnings = ieml::parser::errorManagerToJson(parser.getErrorListener());
    auto context = parser.getContext();
    auto& cregister = context->getCategoryRegister();
    auto& wregister = context->getWordRegister();
    auto& sregister = context->getScriptRegister();
    auto& pregister = context->getParadigmRegister();

    const CharRange default_range(parser.getDefaultFileId(), 0, 0, 0, 0);

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

    nlohmann::json category_hierarchies = nlohmann::json::array();
    for (const auto& table: context->getParadigmRegister().getTableHierarchies()) {
        category_hierarchies.push_back(serializeCategoryHierarchy(*context, table));
    }
    
    for (auto it: sregister.defined_script())
        elements[it.second->uid()] = _scriptToJson(it.second, *context, default_range);

    // serialize table after scripts so all the tables are created in script register
    nlohmann::json tables;
    for (const auto& it: sregister.get_tables()) {
        tables[it.second->uid()] = tableToJson(it.second);
    }
    // category paradigm register
    for (const auto& it: pregister.get_tables()) {
        tables[it.second->uid()] = tableToJson(it.second);
    }


    return {
        {"errors", errors_and_warnings.first},
        {"warnings", errors_and_warnings.second},
        {"language", language._to_string()},
        {"elements", elements},
        {"tables", tables},
        {"category_hierarchies", category_hierarchies}
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


nlohmann::json _wrap(ieml::structure::Script::Ptr ptr, size_t times) {
    if (times == 0) 
        return ptr->uid();
    else {
        auto res = nlohmann::json::array();
        res.push_back(_wrap(ptr, times - 1));
        return res;
    }
};

template<class T>
nlohmann::json ieml::parser::tableToJson(const T& table) {
    nlohmann::json res;

    if (table->get_type() == +ieml::structure::TableType::CELL) {
        // nothing
    } else if (table->get_type() == +ieml::structure::TableType::TABLESET) {
        res["children"] = nlohmann::json::array();
        for (auto child: table->get_sub_tables())
            res["children"].push_back(child->uid());
    } else if (table->get_type() == +ieml::structure::TableType::TABLEND) {
        res["n_dim"] = table->ndim(0);

        res["shape"] = nlohmann::json::array();
        res["headers"] = nlohmann::json::array();
        res["cells"] = nlohmann::json::array();

        const auto shape_tuple = std::get<1>(table->getShape());
        const auto shape = shape_tuple[0];
        for (size_t dim = 0; dim < table->ndim(0); dim++) {
            res["shape"].push_back(shape[dim]);

            auto headers = nlohmann::json::array();
            for (auto h: table->getHeaders(0, dim))
                headers.push_back(h->uid());
                
            res["headers"].push_back(headers);
        }
        
        res["cells"] = nlohmann::json::array();
        for (size_t i = 0; i < shape[0];  i++) {
            res["cells"].push_back(nlohmann::json::array());
            for (size_t j = 0; j < shape[1]; j++) {
                res["cells"][i].push_back(nlohmann::json::array());
                for (size_t k = 0; k < shape[2]; k++) {
                    res["cells"][i][j].push_back(table->at(0, {i, j, k})->uid());
                }
            }
        }
    } else {
        throw "Invalid table type";
    }

    res["id"] = table->uid();
    res["type"] = table->get_type()._to_string();
    res["element_type"] = table->table_name()->getElementType()._to_string();
    res["title"] = table->table_name()->uid();
    res["str"] = table->table_name()->to_string();

    return res;
}