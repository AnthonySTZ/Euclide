#include <gtest/gtest.h>
#include "nodes/geometry/transform.h"

namespace butter {

TEST(Transform, TestTranslation) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({1.0, 0.0, 5.0});
    uint32_t p1 = mesh.addPoint({-1.0, 9.0, -5.0});
    uint32_t p2 = mesh.addPoint({0.0, 10.0, 0.0});

    TransformSettings settings {
        .translation = {1.0, -5.0, 2.5}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], 2.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], -5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], 7.5, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], 4.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], -2.5, 1e-4f);

    EXPECT_NEAR(mesh.points.posX[p2], 1.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p2], 5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p2], 2.5, 1e-4f);
}

TEST(Transform, TestScale) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({1.0, 0.0, 5.0});
    uint32_t p1 = mesh.addPoint({-1.0, 9.0, -5.0});
    uint32_t p2 = mesh.addPoint({0.0, 10.0, 0.0});

    TransformSettings settings {
        .scale = {1.0, -5.0, 2.5}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], 1.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], 12.5, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], -1.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], -45.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], -12.5, 1e-4f);

    EXPECT_NEAR(mesh.points.posX[p2], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p2], -50.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p2], 0.0, 1e-4f);
}

TEST(Transform, TestRotationAroundYAxis) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({1.0, 0.0, 5.0});
    uint32_t p1 = mesh.addPoint({-1.0, 9.0, -5.0});
    uint32_t p2 = mesh.addPoint({0.0, 10.0, 0.0});

    TransformSettings settings {
        .rotation = {0.0, 90.0, 0.0}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], 5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], -1.0, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], -5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], 9.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], 1.0, 1e-4f);

    EXPECT_NEAR(mesh.points.posX[p2], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p2], 10.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p2], 0.0, 1e-4f);
}

TEST(Transform, TestRotationAroundXAxis) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({0.0f, 1.0f, 2.0f});
    uint32_t p1 = mesh.addPoint({5.0f, -3.0f, 4.0f});

    TransformSettings settings{
        .rotation = {90.0f, 0.0f, 0.0f}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], -2.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], 1.0, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], 5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], -4.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], -3.0, 1e-4f);
}

TEST(Transform, TestRotationAroundZAxis) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({0.0f, 1.0f, 2.0f});
    uint32_t p1 = mesh.addPoint({5.0f, -3.0f, 4.0f});

    TransformSettings settings{
        .rotation = {0.0f, 0.0f, 90.0f}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], -1.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], 0.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], 2.0, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], 3.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], 5.0, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], 4.0, 1e-4f);
}

TEST(Transform, TestRotationCombined) {

    Mesh mesh;

    uint32_t p0 = mesh.addPoint({0.0f, 1.0f, 2.0f});
    uint32_t p1 = mesh.addPoint({5.0f, -3.0f, 4.0f});

    TransformSettings settings{
        .rotation = {15.0f, 80.0f, 176.0f}
    };

    Transform::transform(mesh, settings);

    EXPECT_NEAR(mesh.points.posX[p0], -2.18341, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p0], -0.296704, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p0], 0.380406, 1e-4f);
    
    EXPECT_NEAR(mesh.points.posX[p1], -3.62471, 1e-4f);
    EXPECT_NEAR(mesh.points.posY[p1], 4.19612, 1e-4f);
    EXPECT_NEAR(mesh.points.posZ[p1], -4.38794, 1e-4f);
}


}