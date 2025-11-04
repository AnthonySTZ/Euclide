#include <gtest/gtest.h>
#include "mock_node.h"
#include "nodes/scene.h"

#include <memory>

namespace butter {

TEST(Scene, RenameNodeIfNameAlreadyTaken) {

    Scene scene;
    auto node = std::make_shared<CreatePointNode>();
    node->setName("Test");
    auto other_node = std::make_shared<CreatePointNode>();
    other_node->setName("Test");

    scene.addNode(node);
    scene.addNode(other_node);

    EXPECT_EQ(other_node->name(), "Test1");

}

}