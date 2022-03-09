#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/script/MultiplicativeScript.h"
#include "structure/script/AdditiveScript.h"
#include "structure/script/ScriptRegister.h"

#include "ScriptParser.h"
#include "test_utils.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, script_multiplication) {
    auto reg = ScriptRegister();


    const auto substance = reg.get_remarkable_multiplication("wa");
    const auto attribute = reg.get_remarkable_multiplication("u");
    const auto mode = reg.promote(reg.get_remarkable_addition('O'));
    
    const auto res = reg.get_or_create_multiplication({substance, attribute, mode});
    ASSERT_EQ(res->get_layer(), 2);
    ASSERT_EQ(res->get_multiplicity(), 2);
    ASSERT_EQ(res->to_string(), "wa.u.O:.-");
}

TEST(ieml_structure_test_case, script_register_unique) {
    auto reg = ScriptRegister();

    const auto SS_0 = reg.get_or_create_multiplication({reg.get_primitive('S'), reg.get_primitive('S'), reg.get_nullscript(0)});
    const auto SS_1 = reg.get_or_create_multiplication({reg.get_primitive('S'), reg.get_primitive('S'), reg.get_nullscript(0)});
    const auto SSS_0 = reg.get_or_create_multiplication({reg.get_primitive('S'), reg.get_primitive('S'), reg.get_primitive('S')});
    const auto SSS_1 = reg.get_or_create_multiplication({reg.get_primitive('S'), reg.get_primitive('S'), reg.get_primitive('S')});
    ASSERT_NE(SS_0, SSS_0);
    ASSERT_NE(SS_1, SSS_1);
    ASSERT_EQ(SS_0, SS_1);
    ASSERT_EQ(SSS_0, SSS_1);

    const auto uuu_0 = reg.get_or_create_multiplication({reg.get_remarkable_multiplication("u"), reg.get_remarkable_multiplication("u"), reg.get_remarkable_multiplication("u")});
    const auto uuu_1 = reg.get_or_create_multiplication({reg.get_remarkable_multiplication("u"), reg.get_remarkable_multiplication("u"), reg.get_remarkable_multiplication("u")});
    ASSERT_EQ(uuu_1, uuu_0);

    const auto O_0 = reg.get_or_create_addition({reg.get_primitive('S'), reg.get_primitive('T')});
    const auto O_1 = reg.get_or_create_addition({reg.get_primitive('S'), reg.get_primitive('T')});
    ASSERT_EQ(O_0, O_1);
}

TEST(ieml_structure_test_case, test_script_parser) {
    auto listener = ieml::parser::IEMLParserErrorListener();
    auto parser = ieml::parser::ScriptParser("", &listener);
    auto reg = ScriptRegister();

    auto res = parser.parse(&reg, "M:M:.M:M:.-", "", 0, 0);

    ASSERT_NE(res, nullptr);
    ASSERT_EQ(res->to_string(), "M:M:.M:M:.-");
    ASSERT_EQ(res->get_multiplicity(), 81);
    ASSERT_EQ(res->singular_sequences().size(), 81);
}
