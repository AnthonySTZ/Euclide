#include <gtest/gtest.h>
#include "mock_node.h"
#include "nodes/scene.h"

#include <memory>

namespace butter {

TEST(Scene, AddNode) {

    Scene scene;
    auto node = std::make_shared<CreatePointNode>();
    auto other_node = std::make_shared<CreatePointNode>();

    scene.addNode(node);
    scene.addNode(other_node);

    EXPECT_EQ(scene.size(), 2);
}

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

TEST(Scene, GetNodeByName) {

    Scene scene;
    auto node = std::make_shared<CreatePointNode>();
    auto other_node = std::make_shared<CreatePointNode>();

    scene.addNode(node);
    scene.addNode(other_node);

    EXPECT_EQ(scene.node(node->name()), node);
    EXPECT_EQ(scene.node(other_node->name()), other_node);
}

TEST(Scene, ShouldRenturnNullptrIfNodeDoesNotExist) {

    Scene scene;
    auto node = std::make_shared<CreatePointNode>();
    auto other_node = std::make_shared<CreatePointNode>();

    scene.addNode(node);
    scene.addNode(other_node);

    EXPECT_EQ(scene.node("NameThatDoesNotExist"), nullptr);
}

}