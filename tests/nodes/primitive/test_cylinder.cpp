#include <gtest/gtest.h>
#include "nodes/primitive/cylinder.h"

namespace butter {

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

    EXPECT_EQ(mesh.points.size(), 12);
    EXPECT_EQ(mesh.primitives.size(), 6);

    const float posX[12] = {
        6.0, 3.5, -1.5, -4.0, -1.5, 3.5, 1.5, 1.25, 0.75, 0.5, 0.75, 1.25
    };
    const float posY[12] = {
        -0.75, -0.75, -0.75, -0.75, -0.75, -0.75, -3.25, -3.25, -3.25, -3.25, -3.25, -3.25
    };
    const float posZ[12] = { 
        0.5, 4.83013, 4.83013, 0.5, -3.83013, -3.83013, 0.5, 0.933013, 0.933013, 0.5, 0.0669873, 0.0669873
    };
    for (size_t i = 0; i < 12; ++i) {
        EXPECT_NEAR(mesh.points.posX[i], posX[i], 1e-4f);
        EXPECT_NEAR(mesh.points.posY[i], posY[i], 1e-4f);
        EXPECT_NEAR(mesh.points.posZ[i], posZ[i], 1e-4f);
    }
}

}