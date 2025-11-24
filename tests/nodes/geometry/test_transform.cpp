#include <gtest/gtest.h>
#include "nodes/geometry/transform.h"

namespace butter {

TEST(Transform, TestTranslation) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({1.0, 0.0, 5.0});
    uint32_t p1 = mesh.addPoint({-1.0, 9.0, -5.0});
    uint32_t p2 = mesh.addPoint({0.0, 10.0, 0.0});

    TransformSettings settings {
        translation: {1.0, -5.0, 2.5}
    };

    Transform::transform(mesh, settings);

    EXPECT_EQ(mesh.points.posX[p0], 2.0);
    EXPECT_EQ(mesh.points.posY[p0], -5.0);
    EXPECT_EQ(mesh.points.posZ[p0], 7.5);
    
    EXPECT_EQ(mesh.points.posX[p1], 0.0);
    EXPECT_EQ(mesh.points.posY[p1], 4.0);
    EXPECT_EQ(mesh.points.posZ[p1], -2.5);

    EXPECT_EQ(mesh.points.posX[p2], 1.0);
    EXPECT_EQ(mesh.points.posY[p2], 5.0);
    EXPECT_EQ(mesh.points.posZ[p2], 2.5);
}

}