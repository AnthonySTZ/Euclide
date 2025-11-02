#include <gtest/gtest.h>
#include "geometry/points.h"

namespace butter {

TEST(Points, CreatePoints){
    Points p;
    EXPECT_EQ(p.size(), 0);
}

TEST(Points, AddPoint){
    Points p;
    p.addPoint(5.0, 1.0, -3.0);
    EXPECT_EQ(p.size(), 1);
    EXPECT_EQ(p.posX[0], 5.0);
    EXPECT_EQ(p.posY[0], 1.0);
    EXPECT_EQ(p.posZ[0], -3.0);

    EXPECT_EQ(p.normalX.size(), 1);
    EXPECT_EQ(p.normalY.size(), 1);
    EXPECT_EQ(p.normalZ.size(), 1);

    EXPECT_EQ(p.colorR.size(), 1);
    EXPECT_EQ(p.colorG.size(), 1);
    EXPECT_EQ(p.colorB.size(), 1);
}

TEST(Points, AddPointGetIndex){
    Points p;
    size_t first_index = p.addPoint(5.0, 1.0, -3.0);
    size_t second_index = p.addPoint(5.0, 1.0, 8.0);
    EXPECT_EQ(p.size(), 2);
    EXPECT_EQ(first_index, 0);
    EXPECT_EQ(second_index, 1);
}

TEST(PointsTest, ReserveIncreasesCapacity) {
    butter::Points points;

    // Initially, capacity should be 0
    EXPECT_EQ(points.posX.capacity(), 0);

    // Reserve space for 100 points
    points.reserve(100);

    // Capacity should be >= 100
    EXPECT_GE(points.posX.capacity(), 100);
    EXPECT_GE(points.posY.capacity(), 100);
    EXPECT_GE(points.posZ.capacity(), 100);

    // Size should remain 0
    EXPECT_EQ(points.size(), 0);
}

TEST(PointsTest, ReserveDoesNotShrinkCapacity) {
    butter::Points points;
    points.reserve(100);
    size_t oldCap = points.posX.capacity();

    // Reserve a smaller number
    points.reserve(10);

    // Capacity should not decrease
    EXPECT_EQ(points.posX.capacity(), oldCap);
}
    
}