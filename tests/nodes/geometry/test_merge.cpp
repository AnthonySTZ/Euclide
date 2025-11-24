#include <gtest/gtest.h>
#include "nodes/geometry/merge.h"

namespace butter {

TEST(Merge, TestMerge) {

    Mesh mesh1;
    uint32_t p0 = mesh1.addPoint({1.0, 0.0, 5.0});
    uint32_t p1 = mesh1.addPoint({-1.0, 9.0, -5.0});
    uint32_t p2 = mesh1.addPoint({0.0, 10.0, 0.0});
    mesh1.addPrimitive({p0, p1, p2});

    Mesh mesh2;
    uint32_t p3 = mesh2.addPoint({0.0, 0.0, 5.0});
    uint32_t p4 = mesh2.addPoint({1.0, 9.0, -5.0});
    uint32_t p5 = mesh2.addPoint({0.0, 15.0, 0.0});
    mesh2.addPrimitive({p3, p4, p5});

    uint32_t p6 = mesh2.addPoint({0.0, 0.0, 5.0});
    uint32_t p7 = mesh2.addPoint({1.0, 9.0, -5.0});
    uint32_t p8 = mesh2.addPoint({0.0, 15.0, 0.0});
    mesh2.addPrimitive({p6, p7, p8});

    Merge::merge(mesh1, mesh2);

    EXPECT_EQ(mesh1.points.size(), 9);
    EXPECT_EQ(mesh1.primitives.size(), 3);

    EXPECT_EQ(mesh1.getPointIndicesOfPrimitive(0), std::vector<uint32_t>({p0, p1, p2}));
    EXPECT_EQ(mesh1.getPointIndicesOfPrimitive(1), std::vector<uint32_t>({p3 + 3, p4 + 3, p5 + 3}));
    EXPECT_EQ(mesh1.getPointIndicesOfPrimitive(2), std::vector<uint32_t>({p6 + 3, p7 + 3, p8 + 3}));
}

}