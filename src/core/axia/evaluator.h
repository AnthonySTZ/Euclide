#pragma once

#include <cstdint>

#include "parser.h"
#include "geometry/attribute_set.h"
#include "semantic.h"

namespace euclide {

struct EvalContext {
    AttributeSet& attribs;
    uint32_t index;
};

struct AxiaEvaluator : ASTVisitor {
    EvalContext& context;

    std::unordered_map<Symbol*, Value> locals;

    explicit AxiaEvaluator(EvalContext& t_c) : context(t_c) {}

    Value visit(StringLiteral& t_node) override { return t_node.value; }
    Value visit(NumericLiteral& t_node) override { return t_node.value; }
    Value visit(VarDecl& t_node) override { return t_node.name; }
    Value visit(BinaryOp& t_node) override { // TODO:
        const float lhs = std::get<float>(t_node.left->accept(*this));
        const float rhs = std::get<float>(t_node.right->accept(*this));
        switch (t_node.type) {
        case NodeType::AddOp:
            return lhs + rhs;
        case NodeType::SubOp:
            return lhs - rhs;
        case NodeType::MultOp:
            return lhs * rhs;
        case NodeType::DivOp:
            return lhs / rhs;
        default:
            break;
        }
        return 0.0f;
    }
    Value visit(AttributeIdentifier& t_node) override {
        // check t_node.type
        if (t_node.component >= 0) {
            float* ptr = context.attribs.findOrCreate(t_node.name, t_node.type)->component<float>(t_node.component);
            return ptr[context.index];
        }
        return {};
    }
    Value visit(Identifier& t_node) override {
        if (t_node.symbol == nullptr)
            return {};

        auto it = locals.find(t_node.symbol);
        if (it == locals.end()) {
            throw std::runtime_error("Locals must be defined before assigned new value !");
            return {};
        }

        return locals[t_node.symbol];
    }

    Value visit(Assignment& t_node) override {
        Value value = t_node.value->accept(*this);
        if (const AttributeIdentifier* attr = dynamic_cast<const AttributeIdentifier*>(t_node.identifier.get())) {
            std::cout << attr->name << ' ' << attr->component << '\n';
            // TODO:check type
            if (attr->component >= 0) {
                float* ptr = context.attribs.findOrCreate(attr->name, attr->type)->component<float>(attr->component);
                ptr[context.index] = std::get<float>(value);
            }
        } else if (const VarDecl* attr = dynamic_cast<const VarDecl*>(t_node.identifier.get())) {
            // Local var declaration
            const VarDecl* localAttr = dynamic_cast<const VarDecl*>(t_node.identifier.get());
            // TODO: check type
            if (locals.count(localAttr->symbol) > 0) {
                throw std::runtime_error("Redefinition of variable forbidden !");
                return {};
            }
            locals.emplace(localAttr->symbol, std::get<float>(value));

        } else if (const Identifier* attr = dynamic_cast<const Identifier*>(t_node.identifier.get())) {
            // Local var
            const Identifier* localAttr = dynamic_cast<const Identifier*>(t_node.identifier.get());
            // TODO: check type
            auto it = locals.find(t_node.symbol);
            if (it == locals.end()) {
                throw std::runtime_error("Locals must be defined before assigned new value !");
                return {};
            }
            locals.emplace(localAttr->symbol, std::get<float>(value));
        }

        return value;
    }
    Value visit(Statement& t_node) override { return t_node.right->accept(*this); }
};

} // namespace euclide