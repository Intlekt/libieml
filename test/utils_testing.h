#pragma once

#include "IemlParser.h"
#include "structure/script/ScriptRegister.h"
#include "ScriptParser.h"

#include "gtest/gtest.h"

#define PARSE_NO_ERRORS(Str)                                        \
    ieml::parser::IEMLParser parser(Str);                           \
    try {                                                           \
      parser.parse();                                               \
    } catch (std::exception& e) {                                   \
      EXPECT_TRUE(false) << e.what();                               \
    }                                                               \
                                                                    \
    std::ostringstream os;                                          \
    for (auto& error : parser.getSyntaxErrors()) {                  \
      os << error->to_string() << std::endl;                        \
    }                                                               \
                                                                    \
    ASSERT_EQ(parser.getSyntaxErrors().size(), 0) << os.str();      \


namespace ieml::testing {

ieml::structure::Script::Ptr parse_script(ieml::structure::ScriptRegister* reg, const std::string& s);

}
