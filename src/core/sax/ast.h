#pragma once

#include <memory>
#include <string>

namespace euclide {

enum class NodeType { NumericLiteral, StringLiteral, BinaryOp, UndefinedNode };

struct ASTNode {
    NodeType type = NodeType::UndefinedNode;
    explicit ASTNode(const NodeType t_type) : type(t_type) {}
    virtual ~ASTNode() = default;
};

using AST = std::unique_ptr<ASTNode>;

struct StringLiteral : ASTNode {
    std::string value;
    StringLiteral(const std::string t_string) : ASTNode(NodeType::StringLiteral), value(std::move(t_string)) {}
};

struct NumericLiteral : ASTNode {
    double value;
    NumericLiteral(const double t_value) : ASTNode(NodeType::NumericLiteral), value(t_value) {}
};

} // namespace euclide