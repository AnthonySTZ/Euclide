#include <gtest/gtest.h>
#include "nodes/primitive/grid.h"

#include "fields/int2field.h"
#include "fields/float2field.h"
#include "fields/float3field.h"

#include <memory>

namespace euclide {

TEST(Grid, GeneratesCorrectPointCount) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Int2Field>("divisions")->setValue(2, 3);

    auto mesh = grid->cook(0);
    EXPECT_EQ(mesh->points.size(), 12);
}

TEST(Grid, GeneratesCorrectPrimitiveCount) {
    auto grid = std::make_shared<Grid>();

    // 4×5 divisions → 20 quads
    grid->getField<Int2Field>("divisions")->setValue(4, 5);

    auto mesh = grid->cook(0);

    EXPECT_EQ(mesh->primitives.size(), 20);
}

TEST(Grid, CorrectSpacing) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Float3Field>("position")->setValue(0, 0, 0);
    grid->getField<Float2Field>("size")->setValue(4, 4);
    grid->getField<Int2Field>("divisions")->setValue(2, 2);

    auto mesh = grid->cook(0);

    // Expected point grid:
    // x: -2, 0, +2
    // z: -2, 0, +2

    ASSERT_EQ(mesh->points.size(), 9);

    EXPECT_FLOAT_EQ(mesh->points.posX[0], -2);
    EXPECT_FLOAT_EQ(mesh->points.posX[1], 0);
    EXPECT_FLOAT_EQ(mesh->points.posX[2], 2);
}

TEST(Grid, Grid2x4CustomPositionAndSize) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Float3Field>("position")->setValue(10.0, 5.0, 20.0);
    grid->getField<Float2Field>("size")->setValue(4, 6);
    grid->getField<Int2Field>("divisions")->setValue(2, 4); // 2 rows x 4 cols
    grid->getField<NodeField<int>>("orientation")->setValue(GridOrientation::ZX);

    auto mesh = grid->cook(0);
    EXPECT_EQ(mesh->points.size(), 15);

    float expectedX[5] = {8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
    float expectedZ[3] = {17.0f, 20.0f, 23.0f};

    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 5; ++col) {
            const size_t pointIndex = row * 5 + col;
            EXPECT_FLOAT_EQ(mesh->points.posX[pointIndex], expectedX[col]);
            EXPECT_FLOAT_EQ(mesh->points.posY[pointIndex], 5.0f); // center Y
            EXPECT_FLOAT_EQ(mesh->points.posZ[pointIndex], expectedZ[row]);
        }
    }
}

TEST(Grid, Grid2x3_XY) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Float3Field>("position")->setValue(10.0, 5.0, 20.0);
    grid->getField<Float2Field>("size")->setValue(4, 6);
    grid->getField<Int2Field>("divisions")->setValue(2, 3);
    grid->getField<NodeField<int>>("orientation")->setValue(GridOrientation::XY);

    auto mesh = grid->cook(0);
    EXPECT_EQ(mesh->points.size(), 12);

    float expectedX[4] = {8.0f, 28.0f / 3.0f, 32.0f / 3.0f, 12.0f};
    float expectedY[3] = {2.0f, 5.0f, 8.0f};

    float expectedZ = 20.0f;
    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            const size_t idx = row * 4 + col;
            EXPECT_FLOAT_EQ(mesh->points.posX[idx], expectedX[col]);
            EXPECT_FLOAT_EQ(mesh->points.posY[idx], expectedY[row]);
            EXPECT_FLOAT_EQ(mesh->points.posZ[idx], expectedZ);
        }
    }
}

TEST(Grid, Grid2x3_YZ) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Float3Field>("position")->setValue(10.0, 5.0, 20.0);
    grid->getField<Float2Field>("size")->setValue(4, 6);
    grid->getField<Int2Field>("divisions")->setValue(2, 3);
    grid->getField<NodeField<int>>("orientation")->setValue(GridOrientation::YZ);

    auto mesh = grid->cook(0);
    EXPECT_EQ(mesh->points.size(), 12);

    float expectedZ[4] = {18.0f, 58.0f / 3.0f, 62.0f / 3.0f, 22.0f};
    float expectedY[3] = {2.0f, 5.0f, 8.0f};

    float expectedX = 10.0f;
    for (size_t row = 0; row < 3; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            const size_t idx = row * 4 + col;
            EXPECT_FLOAT_EQ(mesh->points.posX[idx], expectedX);
            EXPECT_FLOAT_EQ(mesh->points.posY[idx], expectedY[row]);
            EXPECT_FLOAT_EQ(mesh->points.posZ[idx], expectedZ[col]);
        }
    }
}

TEST(GridTest, PrimitiveVerticesFor2x3Grid) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Float3Field>("position")->setValue(0.0f, 0.0f, 0.0f);
    grid->getField<Float2Field>("size")->setValue(2.0f, 3.0f);
    grid->getField<Int2Field>("divisions")->setValue(2, 3);

    // 0  -  1  -  2  -  3
    // |  0  |  1  |  2  |
    // 4  -  5  -  6  -  7
    // |  3  |  4  |  5  |
    // 8  -  9  -  10 -  11

    auto mesh = grid->cook(0);

    EXPECT_EQ(mesh->primitives.size(), 6);

    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(0), (std::vector<uint32_t>{0, 1, 5, 4}));
    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(1), (std::vector<uint32_t>{1, 2, 6, 5}));
    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(2), (std::vector<uint32_t>{2, 3, 7, 6}));

    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(3), (std::vector<uint32_t>{4, 5, 9, 8}));
    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(4), (std::vector<uint32_t>{5, 6, 10, 9}));
    EXPECT_EQ(mesh->getPointIndicesOfPrimitive(5), (std::vector<uint32_t>{6, 7, 11, 10}));
}

} // namespace euclide