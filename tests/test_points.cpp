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
}

TEST(Points, AddPointGetIndex){
    Points p;
    size_t first_index = p.addPoint(5.0, 1.0, -3.0);
    size_t second_index = p.addPoint(5.0, 1.0, 8.0);
    EXPECT_EQ(p.size(), 2);
    EXPECT_EQ(first_index, 0);
    EXPECT_EQ(second_index, 1);
}
    
}