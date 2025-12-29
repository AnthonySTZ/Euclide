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

void expectAssignment(const AST& node, const std::function<void(const AST&)>& identifierCheck,
                      const std::function<void(const AST&)>& valueCheck) {
    ASSERT_EQ(node->type, NodeType::Assignment);
    const auto* assignedNode = dynamic_cast<const Assignment*>(node.get());
    identifierCheck(assignedNode->identifier);
    valueCheck(assignedNode->value);
}

void expectStatement(const AST& node, NodeType stateType, const std::function<void(const AST&)>& statementCheck) {
    ASSERT_EQ(node->type, stateType);
    const auto* assignedNode = dynamic_cast<const Statement*>(node.get());
    statementCheck(assignedNode->left);
}

TEST(AXIAParser, TestNoStatement) {
    Parser parser{};
    const std::string script = "42";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 0);
}

TEST(AXIAParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectNumericLiteral(node, 42); });
}

TEST(AXIAParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectNumericLiteral(node, 42); });
}

TEST(AXIAParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\";";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectStringLiteral(node, "Test"); });
}

TEST(AXIAParser, TestEmptyStringLiteral) {
    Parser parser{};
    const std::string script = "\"\";";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectStringLiteral(node, ""); });
}

TEST(AXIAParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectStringLiteral(node, "   Test"); });
}

TEST(AXIAParser, TestIdentifier) {
    Parser parser{};
    const std::string script = "myvar;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
                    [](const AST& node) { expectIdentifier(node, "myvar"); });
}

TEST(AXIAParser, TestBasicTerm) {
    Parser parser{};
    const std::string script = "2 * 3;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) {expectBinaryOp(
            node, NodeType::MultOp, 
            [](const AST& left) { expectNumericLiteral(left, 2); },
            [](const AST& right) { expectNumericLiteral(right, 3); });}
    );
    // clang-format on
}

TEST(AXIAParser, TestBasicTermWithIdentifier) {
    Parser parser{};
    const std::string script = "2 * myvar;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectBinaryOp(
            node, NodeType::MultOp, 
            [](const AST& left) { expectNumericLiteral(left, 2); },
            [](const AST& right) { expectIdentifier(right, "myvar"); }); } 
    );
    // clang-format on
}

TEST(AXIAParser, MultipleBasicTerms) {
    Parser parser{};
    const std::string script = "2 * 3 / 1;";

    //      /
    //   *     1
    // 2   3

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) { expectBinaryOp(
        node, NodeType::DivOp, 
        [](const AST& left) { expectBinaryOp(
                                    left, NodeType::MultOp,
                                    [](const AST& l){ expectNumericLiteral(l, 2); },
                                    [](const AST& r){ expectNumericLiteral(r, 3);} 
        ); },
        [](const AST& right) { expectNumericLiteral(right, 1); }); } 
    );
    // clang-format on
}

TEST(AXIAParser, TestBasicExpression) {
    Parser parser{};
    const std::string script = "2 + 3;";

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectBinaryOp(
            node, NodeType::AddOp, 
            [](const AST& left) { expectNumericLiteral(left, 2); },
            [](const AST& right) { expectNumericLiteral(right, 3); }); } 
    );
    // clang-format on
}

TEST(AXIAParser, MultipleBasicExpressions) {
    Parser parser{};
    const std::string script = "2 + 3 - 1;";

    //      /
    //   *     1
    // 2   3

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectBinaryOp(
            node, NodeType::SubOp, 
            [](const AST& left) { expectBinaryOp(
                                        left, NodeType::AddOp,
                                        [](const AST& l){ expectNumericLiteral(l, 2); },
                                        [](const AST& r){ expectNumericLiteral(r, 3);} 
            ); },
            [](const AST& right) { expectNumericLiteral(right, 1); }); }
        );
    // clang-format on
}

TEST(AXIAParser, ExpressionsWithTerms) {
    Parser parser{};
    const std::string script = "2 + 3 * 5;";

    //      +
    //   2     *
    //       3   5

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectBinaryOp(
            node, NodeType::AddOp, 
            [](const AST& left) { expectNumericLiteral(left, 2); },
            [](const AST& right) {  expectBinaryOp(
                                    right, NodeType::MultOp,
                                    [](const AST& l){ expectNumericLiteral(l, 3); },
                                    [](const AST& r){ expectNumericLiteral(r, 5);}
        ); }); }
    );
    // clang-format on
}

TEST(AXIAParser, ExpressionsWithTermsMultFirst) {
    Parser parser{};
    const std::string script = "2 / 3 - 5;";

    //      -
    //   /     5
    // 2   3

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectBinaryOp(
            node, NodeType::SubOp, 
            [](const AST& left) { expectBinaryOp(
                                        left, NodeType::DivOp,
                                        [](const AST& l){ expectNumericLiteral(l, 2); },
                                        [](const AST& r){ expectNumericLiteral(r, 3);} 
            ); },
            [](const AST& right) { expectNumericLiteral(right, 5); }); }
    );
    // clang-format on
}

TEST(AXIAParser, TestBasicAssignment) {
    Parser parser{};
    const std::string script = "var = 2;";

    //      =
    //  var    2

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectAssignment(
            node, 
            [](const AST& identifier) { expectIdentifier(identifier, "var"); },
            [](const AST& value) { expectNumericLiteral(value, 2); }); }
    );
    // clang-format on
}

TEST(AXIAParser, TestComplexAssignment) {
    Parser parser{};
    const std::string script = "var = 2 + 3 * 5;";

    //      =
    //  var    +
    //      2     *
    //          3   5

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement(parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node){ expectAssignment(
        node, 
        [](const AST& identifier) { expectIdentifier(identifier, "var"); },
        [](const AST& value) { expectBinaryOp(value, NodeType::AddOp, 
            [](const AST& left){ expectNumericLiteral(left, 2); },
            [](const AST& right){ expectBinaryOp(right, NodeType::MultOp, 
                [](const AST& nestedL){ expectNumericLiteral(nestedL, 3); },
                [](const AST& nestedR){ expectNumericLiteral(nestedR, 5); } );
            } );
        }); }
    );
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

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement( parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) { expectAssignment( node, 
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

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 2);
    // clang-format off
    expectStatement( parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) { 
            expectNumericLiteral(node, 2);
        } 
    );
    expectStatement( parsedTree[1], NodeType::SemiColonStatement,
        [](const AST& node) { 
            EXPECT_EQ(node, nullptr);
        } 
    );
    // clang-format on
}

TEST(AXIAParser, TestMultipleStatements) {
    Parser parser{};
    const std::string script = "var = 2 + 3 * 5; test = 5;";

    //      ;
    //      =
    //  var    +
    //      2     *
    //          3   5

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 2);
    // clang-format off
    expectStatement( parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) { expectAssignment( node, 
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
    expectStatement( parsedTree[1], NodeType::SemiColonStatement,
        [](const AST& node) { expectAssignment( node, 
            [](const AST& identifier) { expectIdentifier(identifier, "test"); },
            [](const AST& value) { expectNumericLiteral(value, 5); } 
        ); } 
    );

    // clang-format on
}

TEST(AXIAParser, TestParenthesis) {
    Parser parser{};
    const std::string script = "var = (2 + 3) * 5;";

    //      ;
    //      =
    //  var    *
    //      +     5
    //    2   3

    const std::vector<AST> parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.size(), 1);
    // clang-format off
    expectStatement( parsedTree[0], NodeType::SemiColonStatement,
        [](const AST& node) { expectAssignment( node, 
            [](const AST& identifier) { expectIdentifier(identifier, "var"); },
            [](const AST& value) { expectBinaryOp(value, NodeType::MultOp, 
                [](const AST& left){ expectBinaryOp(left, NodeType::AddOp, 
                    [](const AST& addLeft){ expectNumericLiteral(addLeft, 2); },
                    [](const AST& addRight){ expectNumericLiteral(addRight, 3); }
                ); },
                [](const AST& right){ expectNumericLiteral(right, 5); } 
            ); } 
        );} 
    );
    // clang-format on
}

} // namespace euclide