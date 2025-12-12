#include <gtest/gtest.h>
#include "mock_node.h"

#include <memory>

namespace euclide {

TEST(Node, RenameNode) {
    auto node = std::make_shared<CreatePointNode>();
    EXPECT_EQ(node->name(), "Point");

    node->setName("NewNode");
    EXPECT_EQ(node->name(), "NewNode");
}

TEST(Node, CookSingleNode) {
    auto node = std::make_shared<CreatePointNode>();

    auto result = node->cook(0);
    auto positions = result->pointAttribs.find("P");
    float* posX = positions->component<float>(0);
    float* posY = positions->component<float>(1);
    float* posZ = positions->component<float>(2);

    ASSERT_EQ(result->pointAttribs.size(), 1);
    EXPECT_FLOAT_EQ(posX[0], 1.0f);
    EXPECT_FLOAT_EQ(posY[0], 2.0f);
    EXPECT_FLOAT_EQ(posZ[0], 3.0f);
}

TEST(Node, CookTwoConnectedNodes) {
    auto point_node = std::make_shared<CreatePointNode>();
    auto second_node = std::make_shared<TestNode>();

    second_node->setInput(0, point_node);
    auto result = second_node->cook(0);
    auto positions = result->pointAttribs.find("P");
    float* posX = positions->component<float>(0);
    float* posY = positions->component<float>(1);
    float* posZ = positions->component<float>(2);

    ASSERT_EQ(result->pointAttribs.size(), 2);
    EXPECT_FLOAT_EQ(posX[0], 1.0f);
    EXPECT_FLOAT_EQ(posY[0], 2.0f);
    EXPECT_FLOAT_EQ(posZ[0], 3.0f);

    EXPECT_FLOAT_EQ(posX[1], 1.0f);
    EXPECT_FLOAT_EQ(posY[1], 0.0f);
    EXPECT_FLOAT_EQ(posZ[1], 2.0f);
}

TEST(Node, CookOutOfBoundsOutput) {
    auto node = std::make_shared<CreatePointNode>();

    auto result = node->cook(1);
    EXPECT_EQ(result, nullptr);
}

TEST(Node, CookWithoutInputNode) {
    auto node = std::make_shared<TestNode>();

    auto result = node->cook(0);
    EXPECT_EQ(result, nullptr);
}

TEST(Node, CachedMesh) {
    auto node = std::make_shared<CreatePointNode>();

    auto first_result = node->cook(0);
    auto second_result = node->cook(0);
    EXPECT_EQ(first_result, second_result);
}

TEST(Node, MultipleOutput) {
    auto point_node = std::make_shared<CreatePointNode>();
    auto second_node = std::make_shared<TestNode>();
    second_node->setInput(0, point_node);

    auto first_result = second_node->cook(0);
    auto first_positions = first_result->pointAttribs.find("P");
    float* first_posX = first_positions->component<float>(0);
    float* first_posY = first_positions->component<float>(1);
    float* first_posZ = first_positions->component<float>(2);

    EXPECT_EQ(first_result->pointAttribs.size(), 2);
    EXPECT_FLOAT_EQ(first_posX[0], 1.0f);
    EXPECT_FLOAT_EQ(first_posY[0], 2.0f);
    EXPECT_FLOAT_EQ(first_posZ[0], 3.0f);

    EXPECT_FLOAT_EQ(first_posX[1], 1.0f);
    EXPECT_FLOAT_EQ(first_posY[1], 0.0f);
    EXPECT_FLOAT_EQ(first_posZ[1], 2.0f);

    auto second_result = second_node->cook(1);
    auto second_positions = second_result->pointAttribs.find("P");
    float* second_posX = second_positions->component<float>(0);
    float* second_posY = second_positions->component<float>(1);
    float* second_posZ = second_positions->component<float>(2);

    EXPECT_EQ(second_result->pointAttribs.size(), 2);
    EXPECT_FLOAT_EQ(second_posX[0], 1.0f);
    EXPECT_FLOAT_EQ(second_posY[0], 2.0f);
    EXPECT_FLOAT_EQ(second_posZ[0], 3.0f);

    EXPECT_FLOAT_EQ(second_posX[1], -8.0f);
    EXPECT_FLOAT_EQ(second_posY[1], 10.0f);
    EXPECT_FLOAT_EQ(second_posZ[1], 20.0f);
}

TEST(NodeConnectionTest, DeleteInputConnection) {
    auto sourceNode = std::make_shared<TestNode>();
    auto targetNode = std::make_shared<TestNode>();

    // Set up input connection
    targetNode->setInput(0, sourceNode, 0);

    // Verify connection exists
    ASSERT_EQ(targetNode->getInputConnection(0)->sourceNode.lock(), sourceNode);
    ASSERT_EQ(sourceNode->getOutputConnections(0).size(), 1);

    // Delete input connection
    targetNode->deleteInputConnection(0);

    // Check target input is null
    EXPECT_EQ(targetNode->getInputConnection(0), nullptr);

    // Check source output no longer has this connection
    EXPECT_EQ(sourceNode->getOutputConnections(0).size(), 0);
}

TEST(Node, GetFieldByName) {
    auto node = std::make_shared<CreatePointNode>();

    auto posX = node->getField<NodeField<float>>("posX");
    EXPECT_EQ(posX->getValue(), 5.0);

    posX->setValue(1.0);

    auto new_posX = node->getField<NodeField<float>>("posX");
    EXPECT_EQ(posX->getValue(), 1.0);
}

TEST(Node, DirtyIfFieldChanged) {
    auto point_node = std::make_shared<CreatePointNode>();
    auto second_node = std::make_shared<TestNode>();
    second_node->setInput(0, point_node);

    auto first_result = second_node->cook(0);
    auto second_result = second_node->cook(0);
    EXPECT_TRUE(first_result == second_result);

    point_node->getField<NodeField<float>>("posX")->setValue(8.0);
    auto third_result = second_node->cook(0);
    EXPECT_FALSE(first_result == third_result);
}

TEST(Node, SetInputToSameNode) {
    auto node = std::make_shared<TestNode>();
    node->setInput(0, node, 0);
    EXPECT_EQ(node->getInputConnection(0), nullptr);
}

TEST(Node, IsInInputHierarchy) {
    auto node_1 = std::make_shared<TestNode>();
    auto node_2 = std::make_shared<TestNode>();
    auto node_3 = std::make_shared<TestNode>();

    node_2->setInput(0, node_1); // 1 -> 2
    node_3->setInput(0, node_2); // 1 -> 2 -> 3

    EXPECT_TRUE(node_3->isInInputsHierarchy(node_1));
    EXPECT_FALSE(node_1->isInInputsHierarchy(node_3));
}

TEST(Node, TestCyclicConnection) {
    auto node_1 = std::make_shared<TestNode>();
    auto node_2 = std::make_shared<TestNode>();
    auto node_3 = std::make_shared<TestNode>();

    node_2->setInput(0, node_1); // 1 -> 2
    node_3->setInput(0, node_2); // 1 -> 2 -> 3
    node_1->setInput(0, node_3); // 1 -> 2 -> 3 -> 1

    EXPECT_EQ(node_2->getInputConnection(0)->sourceNode.lock(), node_1);
    EXPECT_EQ(node_3->getInputConnection(0)->sourceNode.lock(), node_2);
    EXPECT_EQ(node_1->getInputConnection(0), nullptr);
}

} // namespace euclide