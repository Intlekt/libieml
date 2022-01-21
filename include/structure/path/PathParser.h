#pragma once

#include "structure/path/PathTree.h"
#include "ParserContextManager.h"


namespace ieml::parser {

std::shared_ptr<structure::PathTree> parsePath(parser::ParserContextManager& ctx, const std::string& input_str, bool error_stdout = false);

}