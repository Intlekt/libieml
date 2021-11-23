#include <vector>
#include <memory>
#include <stdexcept>

#include "gtest/gtest.h"

#include "structure/Path.h"

using namespace ieml::structure;


TEST(ieml_structure_test_case, path_serialization) {

    // std::vector<InflexingType> plr{InflexingType::PLURAL};
    // std::vector<InflexingType> plr_sing{InflexingType::PLURAL, InflexingType::SINGULAR};

    // std::vector<std::unique_ptr<PathNode>> nodes;
    // nodes.push_back(std::make_unique<RoleNumberPathNode>(RoleType::ROOT));
    // nodes.push_back(std::make_unique<InflexingPathNode>(plr));
    // nodes.push_back(std::make_unique<WordPathNode>(Word("wa.")));
    // auto path = Path(std::move(nodes));

    // EXPECT_EQ("/0/~PLURAL/'wa.'", path.to_string()) << "Invalid path serialization";

    // nodes.clear();
    // nodes.push_back(std::make_unique<RoleNumberPathNode>(RoleType::ROOT));
    // nodes.push_back(std::make_unique<InflexingPathNode>(plr_sing));
    // nodes.push_back(std::make_unique<WordPathNode>(Word("wa.")));
    // auto path2 = Path(std::move(nodes));

    // EXPECT_EQ("/0/~PLURAL,SINGULAR/'wa.'", path2.to_string()) << "Invalid path serialization";

    // nodes.clear();
    // nodes.push_back(std::make_unique<RoleNumberPathNode>(RoleType::ROOT));
    // nodes.push_back(std::make_unique<AuxiliaryPathNode>(AuxiliaryType::ABOVE));
    // nodes.push_back(std::make_unique<InflexingPathNode>(plr_sing));
    // nodes.push_back(std::make_unique<WordPathNode>(Word("wa.")));
    // auto path3 = Path(std::move(nodes));

    // EXPECT_EQ("/0/*ABOVE/~PLURAL,SINGULAR/'wa.'", path3.to_string()) << "Invalid path serialization";

    // nodes.clear();
    // nodes.push_back(std::make_unique<RoleNumberPathNode>(RoleType::ROOT));
    // nodes.push_back(std::make_unique<AuxiliaryPathNode>(AuxiliaryType::ABOVE));
    // nodes.push_back(std::make_unique<CategoryJunctionPathNode>(JunctionType::AND, 0));
    // nodes.push_back(std::make_unique<WordPathNode>(Word("wa.")));
    // auto path4 = Path(std::move(nodes));

    // EXPECT_EQ("/0/*ABOVE/&AND[0]/'wa.'", path4.to_string()) << "Invalid path serialization";

}

TEST(ieml_structure_test_case, path_invalid) {
    auto root0 = std::make_shared<RoleNumberPathNode>(RoleType::ROOT, nullptr);
    EXPECT_THROW(RoleNumberPathNode(RoleType::ROOT, root0), std::invalid_argument) << "Invalid argument not raised on /0/0";
}
