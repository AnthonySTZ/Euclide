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

TEST(Attributes, CopyFloats) {
    Attribute attr{"P", 3, AttributeType::ATTR_TYPE_FLOAT};

    attr.resize(5);
    float* dataX = static_cast<float*>(attr.data[0]);
    float* dataY = static_cast<float*>(attr.data[1]);
    float* dataZ = static_cast<float*>(attr.data[2]);
    dataX[1] = 8.0f;
    dataY[2] = 3.5f;
    dataZ[3] = -2.0f;

    Attribute attrCopy = attr;
    EXPECT_EQ(attrCopy.name, attr.name);
    EXPECT_EQ(attrCopy.attrSize, attr.attrSize);
    EXPECT_EQ(attrCopy.getType(), attr.getType());

    for (size_t component = 0; component < 3; ++component) {
        float* compData = static_cast<float*>(attr.data[component]);
        float* compDataCopy = static_cast<float*>(attrCopy.data[component]);
        EXPECT_FALSE(compData == compDataCopy); // Should not POINT to same datas
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(compData[i], compDataCopy[i]); // Still stores same VALUES
        }
    }
}

TEST(AttributeSets, findOrCreateAttribute) {
    AttributeSet attrSet{};

    auto attribute = attrSet.findOrCreate("P", 3, AttributeType::ATTR_TYPE_FLOAT);
    EXPECT_FALSE(attribute == nullptr);

    EXPECT_EQ(attribute->name, "P");
    EXPECT_EQ(attribute->attrSize, 3);
    EXPECT_EQ(attribute->getType(), AttributeType::ATTR_TYPE_FLOAT);
}

} // namespace euclide
