#include <vector>
#include <set>
#include <memory>
#include <stdexcept>
#include <functional>
#include <string>

#include "gtest/gtest.h"

#include "structure/table/Table.h"

#include "ScriptParser.h"
#include "utils_testing.h"

#include "TestConfig.h"


using namespace ieml::structure;


TEST(ieml_structure_test_case, test_table_nd) {
    auto table1d = std::make_shared<TableNd_<size_t, 1>>(
        std::vector<size_t>{0, 1, 2, 3, 4}, 
        std::array<size_t, 1>{5}, 
        std::array<std::vector<size_t>, 1>{std::vector<size_t>{0, 1, 2, 3, 4}},
        1
    );
    ASSERT_EQ(table1d->getCellsNumber(), 5);
    ASSERT_EQ(table1d->at(0, {3, 0, 0}), 3);
    ASSERT_EQ(table1d->header_at(0, 0, 3), 3);
    ASSERT_EQ(table1d->table_name(), 1);

    auto table2d = std::make_shared<TableNd_<size_t, 2>>(
        std::vector<size_t>{0, 1, 2, 3, 4, 5}, 
        std::array<size_t, 2>{2, 3}, 
        std::array<std::vector<size_t>, 2>{std::vector<size_t>{0, 1}, std::vector<size_t>{2, 3, 4}},
        2
    );
    ASSERT_EQ(table2d->getCellsNumber(), 6);
    ASSERT_EQ(table2d->at(0, {0, 0, 0}), 0);
    ASSERT_EQ(table2d->at(0, {1, 0, 0}), 1);
    ASSERT_EQ(table2d->at(0, {1, 1, 0}), 3);
    ASSERT_EQ(table2d->at(0, {1, 2, 0}), 5);
    ASSERT_EQ(table2d->header_at(0, 0, 1), 1);
    ASSERT_EQ(table2d->header_at(0, 1, 2), 4);
    ASSERT_EQ(table2d->table_name(), 2);

    auto table3d = std::make_shared<TableNd_<size_t, 3>>(
        std::vector<size_t>{0, 1, 2, 3, 4, 5}, 
        std::array<size_t, 3>{2, 3, 1}, 
        std::array<std::vector<size_t>, 3>{std::vector<size_t>{0, 1}, std::vector<size_t>{2, 3, 4}, std::vector<size_t>{5}},
        3
    );
    ASSERT_EQ(table3d->getCellsNumber(), 6);
    ASSERT_EQ(table3d->at(0, {0, 0, 0}), 0);
    ASSERT_EQ(table3d->at(0, {1, 0, 0}), 1);
    ASSERT_EQ(table3d->at(0, {1, 1, 0}), 3);
    ASSERT_EQ(table3d->at(0, {1, 2, 0}), 5);
    ASSERT_EQ(table3d->header_at(0, 1, 2), 4);
    ASSERT_EQ(table3d->table_name(), 3);

    TableSet<size_t>::Children children{table1d.get(), table2d.get(), table3d.get()};

    const auto table_set = std::make_shared<TableSet<size_t>>(children, 0);
    ASSERT_EQ(table_set->getCellsNumber(), 17);
    ASSERT_EQ(table_set->at(0, {3, 0, 0}), 3);
    ASSERT_EQ(table_set->at(1, {1, 1, 0}), 3);
    ASSERT_EQ(table_set->table_name(), 0);
    ASSERT_EQ(table_set->table_dim_name(0), 1);
    ASSERT_EQ(table_set->table_dim_name(1), 2);
    ASSERT_EQ(table_set->table_dim_name(2), 3);
}

