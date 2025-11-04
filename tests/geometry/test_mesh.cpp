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

}
