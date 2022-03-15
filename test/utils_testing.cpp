#include "utils_testing.h"


ieml::structure::Script::Ptr ieml::testing::parse_script(ieml::structure::ScriptRegister* reg, const std::string& s) {
    ieml::structure::Script::Ptr res = reg->get_defined_script_by_string(s);

    if (!res) {
        auto listener = ieml::parser::IEMLParserErrorListener();
        auto parser = ieml::parser::ScriptParser("Parsing script: " + s, &listener);
        res;
        try {                                                           
            res = parser.get_or_parse(reg, s, "", 0, 0);
        } catch (std::exception& e) {
            EXPECT_TRUE(false) << e.what();
        }

    }

    EXPECT_NE(res, nullptr);
    return res;
}
