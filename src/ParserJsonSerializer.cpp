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

nlohmann::json ieml::parser::syntaxErrorSerializer(const SyntaxError& syntax_error) {
    return {
        {"range", charRangeToJson(syntax_error.getCharRange())},
        {"message", syntax_error.getMessage()}
    };
}

nlohmann::json ieml::parser::errorManagerToJson(const IEMLParserErrorListener& error_manager) {
    nlohmann::json error_list = nlohmann::json::array();
    for (auto& error: error_manager.getSyntaxErrors()) {
        error_list.push_back(syntaxErrorSerializer(*error));
    }
    return error_list;
}

nlohmann::json ieml::parser::parserToJson(const IEMLParser& parser) {
    nlohmann::json j;
    j["errors"] = ieml::parser::errorManagerToJson(parser.getErrorListener());
    j["concepts"] = nlohmann::json::array();

    return j;
};
