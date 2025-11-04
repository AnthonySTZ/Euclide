#include <gtest/gtest.h>
#include "nodes/geometry/cube.h"

#include <memory>

namespace butter {

TEST(Cube, TestCubeName) {
    auto cube = std::make_shared<Cube>();
    EXPECT_EQ(cube->name(), "Cube");
}

TEST(Cube, CubeOutput) {
    auto cube = std::make_shared<Cube>();

    // Set fields
    cube->getField<Float3Field>("position")->setValue(0.0f, 0.0f, 0.0f);
    cube->getField<Float3Field>("size")->setValue(2.0f, 2.0f, 2.0f);

    auto result = cube->cook(0);

    // Check points
    ASSERT_EQ(result->points.size(), 8);
    EXPECT_FLOAT_EQ(result->points.posX[0], 1.0f);
    EXPECT_FLOAT_EQ(result->points.posY[0], 1.0f);
    EXPECT_FLOAT_EQ(result->points.posZ[0], -1.0f);

    EXPECT_FLOAT_EQ(result->points.posX[6], -1.0f);
    EXPECT_FLOAT_EQ(result->points.posY[6], -1.0f);
    EXPECT_FLOAT_EQ(result->points.posZ[6], 1.0f);

    ASSERT_EQ(result->primitives.size(), 6);
}

}