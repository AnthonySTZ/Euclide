#include <gtest/gtest.h>
#include "mock_node.h"

#include <memory>

namespace butter {

TEST(Node, CookSingleNode) {
    auto node = std::make_shared<CreatePointNode>();

    auto result = node->cook(0);
    ASSERT_EQ(result->points.size(), 1);
    EXPECT_FLOAT_EQ(result->points.posX[0], 1.0f);
    EXPECT_FLOAT_EQ(result->points.posY[0], 2.0f);
    EXPECT_FLOAT_EQ(result->points.posZ[0], 3.0f);
}

TEST(Node, CookTwoConnectedNodes) {
    auto point_node = std::make_shared<CreatePointNode>();
    auto second_node = std::make_shared<TestNode>();

    second_node->setInput(0, point_node);
    auto result = second_node->cook(0);

    ASSERT_EQ(result->points.size(), 2);
    EXPECT_FLOAT_EQ(result->points.posX[0], 1.0f);
    EXPECT_FLOAT_EQ(result->points.posY[0], 2.0f);
    EXPECT_FLOAT_EQ(result->points.posZ[0], 3.0f);

    EXPECT_FLOAT_EQ(result->points.posX[1], 1.0f);
    EXPECT_FLOAT_EQ(result->points.posY[1], 0.0f);
    EXPECT_FLOAT_EQ(result->points.posZ[1], 2.0f);
}

TEST(Node, CookOutOfBoundsOutput) {
    auto node = std::make_shared<CreatePointNode>();

    auto result = node->cook(1);
    EXPECT_EQ(result, nullptr);
}

TEST(Node, CookWithoutInputNode) {
    auto node = std::make_shared<TestNode>();

    auto result = node->cook(0);
    EXPECT_EQ(result, nullptr);
}

TEST(Node, CachedMesh) {
    auto node = std::make_shared<CreatePointNode>();

    auto first_result = node->cook(0);
    auto second_result = node->cook(0);
    EXPECT_EQ(first_result, second_result);
}

}