#include <gtest/gtest.h>
#include "nodes/primitive/cylinder.h"

namespace euclide {

TEST(Cylinder, CylinderNotCapped) {
    Mesh mesh;

    const CylinderSettings settings{
        .position = {1.0, -2.0, 0.5},
        .radius = {5.0, 0.5},
        .height = 2.5,
        .divisions = 6,
        .capped = false,
    };

    Cylinder::createCylinder(mesh, settings);

    EXPECT_EQ(mesh.pointAttribs.size(), 12);
    auto positions = mesh.pointAttribs.find("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    EXPECT_EQ(mesh.primitives.size(), 6);

    const float posX[12] = {6.0, 3.5, -1.5, -4.0, -1.5, 3.5, 1.5, 1.25, 0.75, 0.5, 0.75, 1.25};
    const float posY[12] = {-0.75, -0.75, -0.75, -0.75, -0.75, -0.75, -3.25, -3.25, -3.25, -3.25, -3.25, -3.25};
    const float posZ[12] = {0.5, 4.83013,  4.83013,  0.5, -3.83013,  -3.83013,
                            0.5, 0.933013, 0.933013, 0.5, 0.0669873, 0.0669873};
    for (size_t i = 0; i < 12; ++i) {
        EXPECT_NEAR(pointsPosX[i], posX[i], 1e-4f);
        EXPECT_NEAR(pointsPosY[i], posY[i], 1e-4f);
        EXPECT_NEAR(pointsPosZ[i], posZ[i], 1e-4f);
    }

    std::vector<std::vector<uint32_t>> prim = {{0, 1, 7, 6},  {1, 2, 8, 7},   {2, 3, 9, 8},
                                               {3, 4, 10, 9}, {4, 5, 11, 10}, {5, 0, 6, 11}};
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(mesh.getPointIndicesOfPrimitive(i), prim[i]);
    }
}

TEST(Cylinder, CylinderCapped) {
    Mesh mesh;

    const CylinderSettings settings{
        .position = {1.0, -2.0, 0.5},
        .radius = {5.0, 0.5},
        .height = 2.5,
        .divisions = 6,
        .capped = true,
    };

    Cylinder::createCylinder(mesh, settings);

    EXPECT_EQ(mesh.pointAttribs.size(), 12);
    auto positions = mesh.pointAttribs.find("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);
    EXPECT_EQ(mesh.primitives.size(), 8);

    const float posX[12] = {6.0, 3.5, -1.5, -4.0, -1.5, 3.5, 1.5, 1.25, 0.75, 0.5, 0.75, 1.25};
    const float posY[12] = {-0.75, -0.75, -0.75, -0.75, -0.75, -0.75, -3.25, -3.25, -3.25, -3.25, -3.25, -3.25};
    const float posZ[12] = {0.5, 4.83013,  4.83013,  0.5, -3.83013,  -3.83013,
                            0.5, 0.933013, 0.933013, 0.5, 0.0669873, 0.0669873};
    for (size_t i = 0; i < 12; ++i) {
        EXPECT_NEAR(pointsPosX[i], posX[i], 1e-4f);
        EXPECT_NEAR(pointsPosY[i], posY[i], 1e-4f);
        EXPECT_NEAR(pointsPosZ[i], posZ[i], 1e-4f);
    }

    std::vector<std::vector<uint32_t>> prim = {
        {0, 1, 7, 6},   {1, 2, 8, 7},  {2, 3, 9, 8},       {3, 4, 10, 9},
        {4, 5, 11, 10}, {5, 0, 6, 11}, {0, 1, 2, 3, 4, 5}, {6, 7, 8, 9, 10, 11},
    };
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(mesh.getPointIndicesOfPrimitive(i), prim[i]);
    }
}

} // namespace euclide