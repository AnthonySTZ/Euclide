#pragma once

#include <memory>
#include <string>
#include <variant>

namespace euclide {

using Value = std::variant<float, std::string>;

struct LocalVar;

struct StringLiteral;
struct NumericLiteral;
struct VarDecl;
struct BinaryOp;
struct Identifier;
struct Assignment;
struct Statement;

// --------- AST Visitor -------
struct ASTVisitor {
    virtual Value visit(StringLiteral&) = 0;
    virtual Value visit(NumericLiteral&) = 0;
    virtual Value visit(VarDecl&) = 0;
    virtual Value visit(BinaryOp&) = 0;
    virtual Value visit(Identifier&) = 0;
    virtual Value visit(Assignment&) = 0;
    virtual Value visit(Statement&) = 0;
};

enum class NodeType {
    NumericLiteral,
    StringLiteral,
    Identifier,
    MultOp,
    DivOp,
    AddOp,
    SubOp,
    VarDecl,
    Assignment,
    SemiColonStatement,
    UndefinedNode
};

struct ASTNode {
    NodeType type = NodeType::UndefinedNode;
    explicit ASTNode(const NodeType t_type) : type(t_type) {}
    virtual ~ASTNode() = default;
    virtual Value accept(ASTVisitor&) = 0;
};

using AST = std::unique_ptr<ASTNode>;

struct StringLiteral : ASTNode {
    std::string value;
    StringLiteral(const std::string t_string) : ASTNode(NodeType::StringLiteral), value(std::move(t_string)) {}
    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct NumericLiteral : ASTNode {
    float value;
    NumericLiteral(const float t_value) : ASTNode(NodeType::NumericLiteral), value(t_value) {}

    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct Identifier : ASTNode {
    std::string name;
    LocalVar* localvar = nullptr;

    Identifier(const std::string t_name) : ASTNode(NodeType::Identifier), name(std::move(t_name)) {}

    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct VarDecl : ASTNode {
    std::string type;
    std::string name;
    LocalVar* localvar = nullptr;

    VarDecl(const std::string t_type, const std::string t_name)
        : ASTNode(NodeType::VarDecl), type(std::move(t_type)), name(t_name) {}

    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct BinaryOp : ASTNode {
    AST left;
    AST right;

    BinaryOp(const NodeType t_op, AST&& t_l, AST&& t_r) : ASTNode(t_op), left(std::move(t_l)), right(std::move(t_r)) {}
    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct Assignment : ASTNode {
    AST identifier; // Can be either an Identifier or a VarDecl
    AST value;

    LocalVar* localvar = nullptr;

    Assignment(AST&& t_l, AST&& t_r)
        : ASTNode(NodeType::Assignment), identifier(std::move(t_l)), value(std::move(t_r)) {}

    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

struct Statement : ASTNode {
    AST right;

    Statement(const NodeType t_type, AST&& t_r) : ASTNode(t_type), right(std::move(t_r)) {}

    Value accept(ASTVisitor& t_visitor) override { return t_visitor.visit(*this); }
};

} // namespace euclide