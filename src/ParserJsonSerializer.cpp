#include "ParserJsonSerializer.h"


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

nlohmann::json ieml::parser::errorManagerToJson(const IEMLParserErrorListener& error_manager) {
    nlohmann::json error_list = nlohmann::json::array();
    for (auto& error: error_manager.getSyntaxErrors()) {
        error_list.push_back(syntaxErrorToJson(*error));
    }
    return error_list;
}

// nlohmann::json ieml::parser::conceptToJson() {
//     size_t concept_id;
//     ieml::AST::CharRange range;
//     std::string node_type;
//     bool user_defined;

//     std::string content;

//     nlohmann::json translations;
//     nlohmann::json cited_concepts = nlohmann::json::array();
//     nlohmann::json back_references = nlohmann::json::array();

//     return {
//         {"concept_id", concept_id},
//         {"range", charRangeToJson(range)},
//         {"node_type", node_type},
//         {"user_defined", user_defined},
//         {"translations", translations},
//         {"definition", 
//             {"content", content},
//             {"cited_concepts", cited_concepts}
//         },
//         {"back_references", back_references}
//     };
// }


nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    nlohmann::json j;
    j["errors"] = ieml::parser::errorManagerToJson(parser.getErrorListener());
    j["concepts"] = nlohmann::json::array();

    
    

    return j;
};
