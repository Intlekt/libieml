#pragma once

#include <nlohmann/json.hpp>
#include "IemlParser.h"
#include "SyntaxError.h"

#include <memory>
#include <set>

#include "structure/CategoryRegister.h"
#include "structure/path/PathTree.h"
#include "structure/Namespace.h"
#include "SourceMapping.h"

#include "relation/RelationGraph.h"
#include "structure/script/ScriptRegister.h"

namespace ieml::parser {


nlohmann::json charRangeToJson(const CharRange& char_range); 

nlohmann::json syntaxErrorToJson(const SyntaxError& syntax_error);

nlohmann::json nameToJson(const ieml::structure::Name& name);

std::pair<nlohmann::json, nlohmann::json> errorManagerToJson(const IEMLParserErrorListener& error_manager);

nlohmann::json categoryToJson(ieml::structure::PathTree::Ptr, 
                              ieml::parser::ParserContextManager&,
                              ieml::relation::RelationGraph::Register&,
                              ieml::relation::RelationGraph::Graph&);

nlohmann::json parserToJson(const IEMLParser& parser);

nlohmann::json serializeCategoryHierarchy(ieml::parser::ParserContextManager& ctx,
                                          const ieml::structure::TableDefinition::Ptr& table);

// Graph
nlohmann::json serializeNode(const structure::CategoryRegister&, 
                             const structure::WordRegister&,
                             const SourceMapping&,
                             const std::shared_ptr<structure::Element>&);

nlohmann::json binaryGraphToJson(ieml::relation::RelationGraph&,
                                 const structure::CategoryRegister&,
                                 const structure::WordRegister&,
                                 const SourceMapping&);

template<class T>
nlohmann::json tableToJson(const T&);



}