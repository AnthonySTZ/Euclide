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

TEST(Mesh, ComputeHalfEdge) {

    Mesh mesh;
    mesh.addPoint(0, 0, 0);
    mesh.addPoint(0, 0, 0);
    mesh.addPoint(0, 0, 0);
    mesh.addPoint(0, 0, 0);
    mesh.addPoint(0, 0, 0);
    mesh.addPoint(0, 0, 0);

    mesh.addPrimitive({0, 1, 2, 3});
    mesh.addPrimitive({1, 4, 5, 2});

    // 0    -    1     -    4   
    // |    1    |     2    |
    // 3    -    2     -    5

    const auto halfedges = mesh.computeHalfEdges();

    EXPECT_EQ(halfedges.size(), 8);

    EXPECT_EQ(halfedges[0].origin, 0);
    EXPECT_EQ(halfedges[0].next, 1);
    EXPECT_EQ(halfedges[0].prev, 3);
    EXPECT_EQ(halfedges[0].face, 0);
    EXPECT_EQ(halfedges[0].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[1].origin, 1);
    EXPECT_EQ(halfedges[1].next, 2);
    EXPECT_EQ(halfedges[1].prev, 0);
    EXPECT_EQ(halfedges[1].face, 0);
    EXPECT_EQ(halfedges[1].twin, 7);
    
    EXPECT_EQ(halfedges[2].origin, 2);
    EXPECT_EQ(halfedges[2].next, 3);
    EXPECT_EQ(halfedges[2].prev, 1);
    EXPECT_EQ(halfedges[2].face, 0);
    EXPECT_EQ(halfedges[2].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[3].origin, 3);
    EXPECT_EQ(halfedges[3].next, 0);
    EXPECT_EQ(halfedges[3].prev, 2);
    EXPECT_EQ(halfedges[3].face, 0);
    EXPECT_EQ(halfedges[3].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[4].origin, 1);
    EXPECT_EQ(halfedges[4].next, 5);
    EXPECT_EQ(halfedges[4].prev, 7);
    EXPECT_EQ(halfedges[4].face, 1);
    EXPECT_EQ(halfedges[4].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[5].origin, 4);
    EXPECT_EQ(halfedges[5].next, 6);
    EXPECT_EQ(halfedges[5].prev, 4);
    EXPECT_EQ(halfedges[5].face, 1);
    EXPECT_EQ(halfedges[5].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[6].origin, 5);
    EXPECT_EQ(halfedges[6].next, 7);
    EXPECT_EQ(halfedges[6].prev, 5);
    EXPECT_EQ(halfedges[6].face, 1);
    EXPECT_EQ(halfedges[6].twin, HalfEdge::NO_TWIN);

    EXPECT_EQ(halfedges[7].origin, 2);
    EXPECT_EQ(halfedges[7].next, 4);
    EXPECT_EQ(halfedges[7].prev, 6);
    EXPECT_EQ(halfedges[7].face, 1);
    EXPECT_EQ(halfedges[7].twin, 1);
}

}
