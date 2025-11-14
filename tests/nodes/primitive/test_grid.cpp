#include <gtest/gtest.h>
#include "nodes/primitive/grid.h"

#include "fields/int2field.h"

#include <memory>

namespace butter {

TEST(Grid, GeneratesCorrectPointCount) {
    auto grid = std::make_shared<Grid>();

    grid->getField<Int2Field>("divisions")->setValue(2, 3);

    auto mesh = grid->cook(0);
    EXPECT_EQ(mesh->points.size(), 12);
}

}