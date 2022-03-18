#include <iostream>
#include <emscripten/bind.h>


#include "IemlParser.h"
#include "SyntaxError.h"
#include "ParserJsonSerializer.h"


std::string parse_project(const std::vector<std::string> & file_ids, const std::vector<std::string> & file_contents)
{
    auto parser = ieml::parser::IEMLParser(file_ids, file_contents);
    parser.parse();
    return ieml::parser::parserToJson(parser).dump();
}

EMSCRIPTEN_BINDINGS(parser) {
    emscripten::register_vector<std::string>("StringList");
    emscripten::function("parse_project", &parse_project);
}
