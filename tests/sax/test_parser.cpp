#include <gtest/gtest.h>
#include "sax/parser.h"

namespace euclide {

void expectNumericLiteral(const AST& node, double expected) {
    ASSERT_EQ(node->type, NodeType::NumericLiteral);
    const auto* lit = dynamic_cast<const NumericLiteral*>(node.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, expected);
}

void expectStringLiteral(const AST& node, const std::string& expected) {
    ASSERT_EQ(node->type, NodeType::StringLiteral);
    const auto* lit = dynamic_cast<const StringLiteral*>(node.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->value, expected);
}

void expectBinaryOp(const AST& node, NodeType opType, const std::function<void(const AST&)>& leftCheck,
                    const std::function<void(const AST&)>& rightCheck) {
    ASSERT_EQ(node->type, opType);
    const auto* binOp = dynamic_cast<const BinaryOp*>(node.get());
    ASSERT_NE(binOp, nullptr);
    leftCheck(binOp->left);
    rightCheck(binOp->right);
}

TEST(SAXParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42";

    const AST parsedTree = parser.parse(script);
    expectNumericLiteral(parsedTree, 42);
}

TEST(SAXParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ";

    const AST parsedTree = parser.parse(script);
    expectNumericLiteral(parsedTree, 42);
}

TEST(SAXParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\"";

    const AST parsedTree = parser.parse(script);
    expectStringLiteral(parsedTree, "Test");
}

TEST(SAXParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ";

    const AST parsedTree = parser.parse(script);
    expectStringLiteral(parsedTree, "   Test");
}

TEST(SAXParser, TestBasicTerm) {
    Parser parser{};
    const std::string script = "2 * 3";

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::MultOp, 
        [](const AST& left) { expectNumericLiteral(left, 2); },
        [](const AST& right) { expectNumericLiteral(right, 3); });
    // clang-format on
}

TEST(SAXParser, MultipleBasicTerms) {
    Parser parser{};
    const std::string script = "2 * 3 / 1";

    //      /
    //   *     1
    // 2   3

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::DivOp, 
        [](const AST& left) { expectBinaryOp(
                                    left, NodeType::MultOp,
                                    [](const AST& l){ expectNumericLiteral(l, 2); },
                                    [](const AST& r){ expectNumericLiteral(r, 3);} 
        ); },
        [](const AST& right) { expectNumericLiteral(right, 1); });
    // clang-format on
}

} // namespace euclide