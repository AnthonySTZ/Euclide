#include <gtest/gtest.h>
#include "nodes/geometry/cube.h"

#include <memory>

namespace butter {

TEST(Cube, TestCubeName) {

    auto cube = std::make_shared<Cube>();
    EXPECT_EQ(cube->name(), "Cube");

}

}