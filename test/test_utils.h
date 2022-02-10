#pragma once

#include "IemlParser.h"


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

