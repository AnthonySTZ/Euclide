#include <gtest/gtest.h>
#include "fields/float3field.h"

#include <memory>

namespace butter {

TEST(Float3Field, CreateFloat3Field) {
    Float3Field field{0.0, 2.0, 8.0};

    vec3 value = field.getValue();
    vec3 expected{0.0, 2.0, 8.0};

    EXPECT_EQ(value, expected);
}

TEST(Float3Field, SetValueByFloat) {
    Float3Field field{1.0, 2.0, 3.0};

    field.setValue(-1.0, 5.0, 6.5);
    vec3 value = field.getValue();
    vec3 expected{-1.0, 5.0, 6.5};

    EXPECT_EQ(value, expected);
}

}