#include <gtest/gtest.h>
#include "axia/parser.h"

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

void expectIdentifier(const AST& node, const std::string& expected) {
    ASSERT_EQ(node->type, NodeType::Identifier);
    const auto* lit = dynamic_cast<const Identifier*>(node.get());
    ASSERT_NE(lit, nullptr);
    EXPECT_EQ(lit->name, expected);
}

void expectBinaryOp(const AST& node, NodeType opType, const std::function<void(const AST&)>& leftCheck,
                    const std::function<void(const AST&)>& rightCheck) {
    ASSERT_EQ(node->type, opType);
    const auto* binOp = dynamic_cast<const BinaryOp*>(node.get());
    ASSERT_NE(binOp, nullptr);
    leftCheck(binOp->left);
    rightCheck(binOp->right);
}

void expectAssignement(const AST& node, const std::function<void(const AST&)>& identifierCheck,
                       const std::function<void(const AST&)>& valueCheck) {
    ASSERT_EQ(node->type, NodeType::Assignement);
    const auto* assignedNode = dynamic_cast<const Assignement*>(node.get());
    identifierCheck(assignedNode->identifier);
    valueCheck(assignedNode->value);
}

void expectStatement(const AST& node, NodeType stateType, const std::function<void(const AST&)>& statementCheck) {
    ASSERT_EQ(node->type, stateType);
    const auto* assignedNode = dynamic_cast<const Statement*>(node.get());
    statementCheck(assignedNode->left);
}

TEST(AXIAParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42";

    const AST parsedTree = parser.parse(script);
    expectNumericLiteral(parsedTree, 42);
}

TEST(AXIAParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ";

    const AST parsedTree = parser.parse(script);
    expectNumericLiteral(parsedTree, 42);
}

TEST(AXIAParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\"";

    const AST parsedTree = parser.parse(script);
    expectStringLiteral(parsedTree, "Test");
}

TEST(AXIAParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ";

    const AST parsedTree = parser.parse(script);
    expectStringLiteral(parsedTree, "   Test");
}

TEST(AXIAParser, TestIdentifier) {
    Parser parser{};
    const std::string script = "myvar";

    const AST parsedTree = parser.parse(script);
    expectIdentifier(parsedTree, "myvar");
}

TEST(AXIAParser, TestBasicTerm) {
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

TEST(AXIAParser, TestBasicTermWithIdentifier) {
    Parser parser{};
    const std::string script = "2 * myvar";

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::MultOp, 
        [](const AST& left) { expectNumericLiteral(left, 2); },
        [](const AST& right) { expectIdentifier(right, "myvar"); });
    // clang-format on
}

TEST(AXIAParser, MultipleBasicTerms) {
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

TEST(AXIAParser, TestBasicExpression) {
    Parser parser{};
    const std::string script = "2 + 3";

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::AddOp, 
        [](const AST& left) { expectNumericLiteral(left, 2); },
        [](const AST& right) { expectNumericLiteral(right, 3); });
    // clang-format on
}

TEST(AXIAParser, MultipleBasicExpressions) {
    Parser parser{};
    const std::string script = "2 + 3 - 1";

    //      /
    //   *     1
    // 2   3

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::SubOp, 
        [](const AST& left) { expectBinaryOp(
                                    left, NodeType::AddOp,
                                    [](const AST& l){ expectNumericLiteral(l, 2); },
                                    [](const AST& r){ expectNumericLiteral(r, 3);} 
        ); },
        [](const AST& right) { expectNumericLiteral(right, 1); });
    // clang-format on
}

TEST(AXIAParser, ExpressionsWithTerms) {
    Parser parser{};
    const std::string script = "2 + 3 * 5";

    //      +
    //   2     *
    //       3   5

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::AddOp, 
        [](const AST& left) { expectNumericLiteral(left, 2); },
        [](const AST& right) {  expectBinaryOp(
                                    right, NodeType::MultOp,
                                    [](const AST& l){ expectNumericLiteral(l, 3); },
                                    [](const AST& r){ expectNumericLiteral(r, 5);} 
        ); });
    // clang-format on
}

TEST(AXIAParser, ExpressionsWithTermsMultFirst) {
    Parser parser{};
    const std::string script = "2 / 3 - 5";

    //      -
    //   /     5
    // 2   3

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectBinaryOp(
        parsedTree, NodeType::SubOp, 
        [](const AST& left) { expectBinaryOp(
                                    left, NodeType::DivOp,
                                    [](const AST& l){ expectNumericLiteral(l, 2); },
                                    [](const AST& r){ expectNumericLiteral(r, 3);} 
        ); },
        [](const AST& right) { expectNumericLiteral(right, 5); });
    // clang-format on
}

TEST(AXIAParser, TestBasicAssignement) {
    Parser parser{};
    const std::string script = "var = 2";

    //      =
    //  var    2

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectAssignement(
        parsedTree, 
        [](const AST& identifier) { expectIdentifier(identifier, "var"); },
        [](const AST& value) { expectNumericLiteral(value, 2); });
    // clang-format on
}

TEST(AXIAParser, TestComplexAssignement) {
    Parser parser{};
    const std::string script = "var = 2 + 3 * 5";

    //      =
    //  var    +
    //      2     *
    //          3   5

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectAssignement(
        parsedTree, 
        [](const AST& identifier) { expectIdentifier(identifier, "var"); },
        [](const AST& value) { expectBinaryOp(value, NodeType::AddOp, 
            [](const AST& left){ expectNumericLiteral(left, 2); },
            [](const AST& right){ expectBinaryOp(right, NodeType::MultOp, 
                [](const AST& nestedL){ expectNumericLiteral(nestedL, 3); },
                [](const AST& nestedR){ expectNumericLiteral(nestedR, 5); } );
            } );
        });
    // clang-format on
}

TEST(AXIAParser, TestComplexStatement) {
    Parser parser{};
    const std::string script = "var = 2 + 3 * 5;";

    //      ;
    //      =
    //  var    +
    //      2     *
    //          3   5

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectStatement( parsedTree, NodeType::SemiColonStatement,
        [](const AST& node) { expectAssignement( node, 
            [](const AST& identifier) { expectIdentifier(identifier, "var"); },
            [](const AST& value) { expectBinaryOp(value, NodeType::AddOp, 
                [](const AST& left){ expectNumericLiteral(left, 2); },
                [](const AST& right){ expectBinaryOp(right, NodeType::MultOp, 
                    [](const AST& nestedL){ expectNumericLiteral(nestedL, 3); },
                    [](const AST& nestedR){ expectNumericLiteral(nestedR, 5); } );
                } );
            } 
        );} 
    );
    // clang-format on
}

TEST(AXIAParser, TestMultipleSemicolon) {
    Parser parser{};
    const std::string script = "2;;";

    //      ;
    //      ;
    //      2

    const AST parsedTree = parser.parse(script);
    // clang-format off
    expectStatement( parsedTree, NodeType::SemiColonStatement,
        [](const AST& node) { 
                expectStatement( node, NodeType::SemiColonStatement, 
                    [](const AST& nested) { expectNumericLiteral(nested, 2); 
            }); 
        } 
    );
    // clang-format on
}

} // namespace euclide