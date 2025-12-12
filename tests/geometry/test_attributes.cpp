#include <gtest/gtest.h>
#include "geometry/attributes.h"

namespace euclide {

TEST(Attributes, InitAttribute) {
    Attribute attr{"P", 3, AttributeType::ATTR_TYPE_FLOAT};

    EXPECT_EQ(attr.name, "P");
    EXPECT_EQ(attr.attrSize, 3);
    EXPECT_EQ(attr.getType(), AttributeType::ATTR_TYPE_FLOAT);
}

TEST(Attributes, ResizeFloat) {
    Attribute attr{"P", 3, AttributeType::ATTR_TYPE_FLOAT};

    attr.resize(5);

    EXPECT_EQ(attr.size, 5);
    for (size_t component = 0; component < 3; ++component) {
        float* compData = static_cast<float*>(attr.data[component]);
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(compData[i], 0.0f);
        }
    }
}

} // namespace euclide
