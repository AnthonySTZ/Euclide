#include <gtest/gtest.h>
#include "geometry/attributes.h"

namespace euclide {

TEST(Attributes, CreateAttribute) {
    Attribute attr{"P", 3, AttributeType::ATTR_TYPE_FLOAT};

    EXPECT_EQ(attr.name, "P");
    EXPECT_EQ(attr.attrSize, 3);
    EXPECT_EQ(attr.getType(), AttributeType::ATTR_TYPE_FLOAT);
}

} // namespace euclide
