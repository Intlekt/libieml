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
#include "utils_testing.h"

#include "TestConfig.h"


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


TEST(ieml_structure_test_case, test_table_from_script) {
    auto reg = ScriptRegister();
    auto res = ieml::testing::parse_script(&reg, "p.E:F:E:+S:+B:+T:.-");
    auto table = reg.get_or_create_table(res);
}

TEST(ieml_structure_test_case, test_all_scripts) {
    auto reg = ScriptRegister();

    std::vector<Script::Ptr> lower;

    std::ifstream exampleFile(TEST_IEML_ALL_SCRIPTS_FILE);
    std::string exampleString, line;
    while (std::getline(exampleFile, line)) {
        auto res = ieml::testing::parse_script(&reg, line);
        EXPECT_NE(res, nullptr) << "Cannot parse " << line;

        if (res) {
            EXPECT_EQ(res->to_string(), line);

            auto table = reg.get_or_create_table(res);
            EXPECT_NE(table, nullptr) << "Cannot build table " << line;

            auto values = table->getCells();
            auto cells = Script::Set(values.begin(), values.end());
            EXPECT_EQ(res->singular_sequences(), cells);

            for (auto l: lower) {
                EXPECT_LT(*l, *res) << "Not lower expected: " << l->to_string() << " < " << res->to_string();
            }
            lower.push_back(res);
        }
    }
    exampleFile.close();
}

TEST(ieml_structure_test_case, test_table_from_script_MMa) {
    auto reg = ScriptRegister();

    auto res = ieml::testing::parse_script(&reg, "S:M:.e.-M:M:.u.-E:.-+wa.e.-'+B:M:.e.-M:M:.a.-E:.-+wa.e.-'+T:M:.e.-M:M:.i.-E:.-+wa.e.-'");
    
    auto table = reg.get_or_create_table(res);
    auto shape = table->getShape();
    auto shape_ref_nd = std::array<size_t, 3>{3, 9, 2};
    auto shape_ref_ = std::vector<std::array<size_t, 3>>{shape_ref_nd, shape_ref_nd, shape_ref_nd};
    auto shape_ref = Table<size_t>::Shape{3, shape_ref_};
    EXPECT_EQ(shape, shape_ref);
}
