#include <gtest/gtest.h>
#include "sax/parser.h"

namespace euclide {

TEST(SAXParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(parsedTree.get())->value, 42);
}

TEST(SAXParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(parsedTree.get())->value, 42);
}

TEST(SAXParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\"";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::StringLiteral);
    EXPECT_EQ(dynamic_cast<const StringLiteral*>(parsedTree.get())->value, "Test");
}

TEST(SAXParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::StringLiteral);
    EXPECT_EQ(dynamic_cast<const StringLiteral*>(parsedTree.get())->value, "   Test");
}

TEST(SAXParser, TestBasicExpression) {
    Parser parser{};
    const std::string script = "2 + 3";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::AddOp);

    const BinaryOp* op = dynamic_cast<const BinaryOp*>(parsedTree.get());
    EXPECT_EQ(op->left->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(op->left.get())->value, 2);
    EXPECT_EQ(op->right->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(op->right.get())->value, 3);
}

TEST(SAXParser, MultipleBasicExpressions) {
    Parser parser{};
    const std::string script = "2 + 3 - 1";

    //      -
    //   +     1
    // 2   3

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::SubOp);

    const BinaryOp* op = dynamic_cast<const BinaryOp*>(parsedTree.get());
    EXPECT_EQ(op->left->type, NodeType::AddOp);
    const BinaryOp* addOp = dynamic_cast<const BinaryOp*>(op->left.get());
    EXPECT_EQ(addOp->left->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(addOp->left.get())->value, 2);
    EXPECT_EQ(addOp->right->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(addOp->right.get())->value, 3);

    EXPECT_EQ(op->right->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(op->right.get())->value, 1);
}

} // namespace euclide