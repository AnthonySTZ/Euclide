#include <gtest/gtest.h>
#include "fields/node_field.h"

#include <memory>

namespace butter {

TEST(Field, CreateDefaultNodeField) {
    NodeField<int> field{0};

    EXPECT_EQ(field.getValue(), 0);
    
    field.setValue(5);
    EXPECT_EQ(field.getValue(), 5);
}

}