#include <gtest/gtest.h>
#include "geometry/attribute_set.h"

namespace euclide {

TEST(Attributes, InitAttribute) {
    TypedAttribute<float, 3> attr{"P"};

    EXPECT_EQ(attr.name(), "P");
    EXPECT_EQ(attr.attrSize(), 3);
    EXPECT_EQ(attr.type(), AttributeType::ATTR_TYPE_FLOAT);
}

TEST(Attributes, ResizeFloat) {
    TypedAttribute<float, 3> attr{"P"};

    attr.resize(5);

    EXPECT_EQ(attr.size(), 5);
    for (size_t c = 0; c < 3; ++c) {
        float* compData = static_cast<float*>(attr.componentRaw(c));
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(compData[i], 0.0f);
        }
    }
}

TEST(Attributes, CopyFloats) {
    TypedAttribute<float, 3> attr{"P"};

    attr.resize(5);
    float* dataX = static_cast<float*>(attr.componentRaw(0));
    float* dataY = static_cast<float*>(attr.componentRaw(1));
    float* dataZ = static_cast<float*>(attr.componentRaw(2));
    dataX[1] = 8.0f;
    dataY[2] = 3.5f;
    dataZ[3] = -2.0f;

    TypedAttribute<float, 3> attrCopy = attr;
    EXPECT_EQ(attrCopy.name(), attr.name());
    EXPECT_EQ(attrCopy.attrSize(), attr.attrSize());
    EXPECT_EQ(attrCopy.type(), attr.type());

    for (size_t c = 0; c < 3; ++c) {
        float* compData = static_cast<float*>(attr.componentRaw(c));
        float* compDataCopy = static_cast<float*>(attrCopy.componentRaw(c));
        EXPECT_FALSE(compData == compDataCopy); // Should not POINT to same datas
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(compData[i], compDataCopy[i]); // Still stores same VALUES
        }
    }
}

TEST(AttributeSets, findOrCreateAttribute) {
    AttributeSet attrSet{};

    auto attribute = attrSet.findOrCreate<float, 3>("P");
    EXPECT_FALSE(attribute == nullptr);

    EXPECT_EQ(attribute->name(), "P");
    EXPECT_EQ(attribute->attrSize(), 3);
    EXPECT_EQ(attribute->type(), AttributeType::ATTR_TYPE_FLOAT);
}

TEST(AttributeSets, findAttribute) {
    AttributeSet attrSet{};

    EXPECT_EQ(attrSet.find("P"), nullptr);

    auto attribute = attrSet.findOrCreate<float, 3>("P");
    EXPECT_FALSE(attribute == nullptr);

    EXPECT_EQ(attrSet.find("P"), attribute);
}

TEST(AttributeSets, findMultipleAttributes) {
    AttributeSet attrSet{};

    EXPECT_EQ(attrSet.find("P"), nullptr);
    EXPECT_EQ(attrSet.find("Cd"), nullptr);

    auto attributeP = attrSet.findOrCreate<float, 3>("P");
    auto attributeCd = attrSet.findOrCreate<float, 3>("Cd");

    EXPECT_EQ(attrSet.find("P"), attributeP);
    EXPECT_EQ(attrSet.find("Cd"), attributeCd);
}

} // namespace euclide
