#include <gtest/gtest.h>
#include "nodes/primitive/cube.h"

#include <memory>

namespace euclide {

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
    auto positions = result->pointAttribs.find("P");
    float* posX = positions->component<float>(0);
    float* posY = positions->component<float>(1);
    float* posZ = positions->component<float>(2);

    // Check points
    ASSERT_EQ(result->pointAttribs.size(), 8);
    EXPECT_FLOAT_EQ(posX[0], 1.0f);
    EXPECT_FLOAT_EQ(posY[0], 1.0f);
    EXPECT_FLOAT_EQ(posZ[0], -1.0f);

    EXPECT_FLOAT_EQ(posX[6], -1.0f);
    EXPECT_FLOAT_EQ(posY[6], -1.0f);
    EXPECT_FLOAT_EQ(posZ[6], 1.0f);

    ASSERT_EQ(result->primitives.size(), 6);
}

} // namespace euclide