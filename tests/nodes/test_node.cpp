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

}