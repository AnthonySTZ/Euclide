#include "evaluator.h"

namespace euclide {

Value AxiaEvaluator::visit(StringLiteral& t_node) {
    return t_node.value;
}

Value AxiaEvaluator::visit(NumericLiteral& t_node) {
    return t_node.value;
}

Value AxiaEvaluator::visit(VarDecl& t_node) {
    return t_node.name;
}

Value AxiaEvaluator::visit(BinaryOp& t_node) {
    // TODO:
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

Value AxiaEvaluator::visit(AttributeIdentifier& t_node) {
    // check t_node.type
    int component = t_node.type == "f" || t_node.type == "float" ? 0 : t_node.component;
    if (component >= 0) {
        float* ptr = context.attribs.findOrCreate(t_node.name, t_node.type)->component<float>(component);
        return ptr[context.index];
    }
    return {};
}

Value AxiaEvaluator::visit(Identifier& t_node) {
    if (t_node.symbol == nullptr)
        return {};

    auto it = locals.find(t_node.symbol);
    if (it == locals.end()) {
        throw std::runtime_error("Locals must be defined before assigned new value !");
        return {};
    }

    return locals[t_node.symbol];
}

Value AxiaEvaluator::visit(Assignment& t_node) {
    Value value = t_node.value->accept(*this);
    switch (t_node.identifier->type) {
    case NodeType::AttributeIdentifier: {
        const AttributeIdentifier* attr = dynamic_cast<const AttributeIdentifier*>(t_node.identifier.get());
        if (!attr)
            throw std::runtime_error("Invalid identifier node");
        int component = attr->type == "f" || attr->type == "float" ? 0 : attr->component;
        if (component >= 0) {
            float* ptr = context.attribs.findOrCreate(attr->name, attr->type)->component<float>(component);
            ptr[context.index] = std::get<float>(value);
        }
    } break;

    case NodeType::VarDecl: {
        // Local var declaration
        const VarDecl* localAttr = dynamic_cast<const VarDecl*>(t_node.identifier.get());
        if (!localAttr)
            throw std::runtime_error("Invalid identifier node");
        // TODO: check type
        if (locals.count(localAttr->symbol) > 0) {
            throw std::runtime_error("Redefinition of variable forbidden !");
        }
        locals.emplace(localAttr->symbol, std::get<float>(value));
    } break;

    case NodeType::Identifier: {
        // Local var
        const Identifier* localAttr = dynamic_cast<const Identifier*>(t_node.identifier.get());
        if (!localAttr)
            throw std::runtime_error("Invalid identifier node");

        // TODO: check type
        auto it = locals.find(localAttr->symbol);
        if (it == locals.end()) {
            throw std::runtime_error("Locals must be defined before assigned new value !");
        }
        it->second = std::get<float>(value);
    } break;

    default:
        throw std::runtime_error("Unexpected assignment !");
    }

    return value;
}

Value AxiaEvaluator::visit(Statement& t_node) {
    return t_node.right->accept(*this);
}

} // namespace euclide