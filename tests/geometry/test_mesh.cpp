#include <gtest/gtest.h>
#include "geometry/mesh.h"

namespace butter {

TEST(Mesh, CreateTriangle){
    Mesh mesh;

    uint32_t p0 = mesh.addPoint(1.0, 5.0, 2.0);
    uint32_t p1 = mesh.addPoint(2.0, 5.0, -8.0);
    uint32_t p2 = mesh.addPoint(-1.0, 5.0, 2.0);

    EXPECT_EQ(mesh.points.size(), 3);
    EXPECT_EQ(mesh.primitives.size(), 0);
    EXPECT_EQ(mesh.vertices.size(), 0);
    
    mesh.addPrimitive({p0, p1, p2});
    
    EXPECT_EQ(mesh.primitives.size(), 1);
    EXPECT_EQ(mesh.vertices.size(), 3);
}

TEST(Mesh, GetMeshCenter){
    Mesh mesh;

    mesh.addPoint(1.0, 5.0, 2.0);
    mesh.addPoint(2.0, 5.0, -8.0);
    mesh.addPoint(-1.0, 5.0, 2.0);
    mesh.addPoint(-18.0, 2.5, 1.0);
    mesh.addPoint(0.0, -5.0, 2.0);
    mesh.addPoint(-1.4, 9.0, -8.0);

    float3 expected {
        -2.9f, 3.58333f, -1.5f
    };

    float3 center = mesh.center();
    float x = center[0];
    float y = center[1];
    float z = center[2];

    EXPECT_NEAR(x, expected[0], 1e-2f);
    EXPECT_NEAR(y, expected[1], 1e-2f);
    EXPECT_NEAR(z, expected[2], 1e-2f);
}

TEST(Mesh, GetMeshCenterMoreThan8Points){
    Mesh mesh;

    mesh.addPoint(1.0, 5.0, 2.0);
    mesh.addPoint(2.0, 5.0, -8.0);
    mesh.addPoint(-1.0, 5.0, 2.0);
    mesh.addPoint(-18.0, 2.5, 1.0);
    mesh.addPoint(0.0, -5.0, 2.0);
    mesh.addPoint(-1.4, 9.0, -8.0);
    mesh.addPoint(1.0, 5.0, 2.0);
    mesh.addPoint(2.0, 5.0, -8.0);
    mesh.addPoint(-1.0, 5.0, 2.0);
    mesh.addPoint(-18.0, 2.5, 1.0);
    mesh.addPoint(0.0, -5.0, 2.0);
    mesh.addPoint(-1.4, 9.0, -8.0);

    float3 expected {
        -2.9f, 3.58333f, -1.5f
    };

    float3 center = mesh.center();
    float x = center[0];
    float y = center[1];
    float z = center[2];

    EXPECT_NEAR(x, expected[0], 1e-2f);
    EXPECT_NEAR(y, expected[1], 1e-2f);
    EXPECT_NEAR(z, expected[2], 1e-2f);
}

}
