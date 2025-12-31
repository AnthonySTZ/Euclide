#pragma once

#include "ast.h"
#include "geometry/attribute_types.h"
#include <unordered_map>
#include <iostream>

namespace euclide {

struct Symbol {
    std::string name;
    AttributeType type;
};

struct Scope {
    Scope* parent;
    std::unordered_map<std::string, Symbol> symbols;

    Symbol* lookup(const std::string& name) {
        if (symbols.count(name))
            return &symbols[name];
        return parent ? parent->lookup(name) : nullptr;
    }
};

struct AxiaSemantic : ASTVisitor {
    Scope* current = new Scope{};

    inline void enterScope() { current = new Scope{current}; }
    inline void exitScope() { current = current->parent; }
    inline Symbol* defineSymbol(const std::string& t_type, const std::string& t_name) {
        if (current->symbols.count(t_name))
            throw std::runtime_error("Redeclaration of " + t_name);

        return &current->symbols.emplace(t_name, Symbol{t_name, AttributeTypeFromString(t_type)}).first->second;
    }

    virtual Value visit(StringLiteral&) override { return {}; }
    virtual Value visit(NumericLiteral&) override { return {}; }
    virtual Value visit(BinaryOp& t_node) override {
        t_node.left->accept(*this);
        t_node.right->accept(*this);
        return {};
    }
    virtual Value visit(AttributeIdentifier&) override { return {}; }
    virtual Value visit(Statement& t_node) override { return t_node.right->accept(*this); }

    Value visit(Identifier& t_node) override {
        Symbol* symbol = current->lookup(t_node.name);
        if (!symbol)
            throw std::runtime_error("Undefined identifier: " + t_node.name);

        t_node.symbol = symbol;
        return {};
    }

    Value visit(Assignment& t_node) override {
        t_node.identifier->accept(*this);
        t_node.value->accept(*this);
        return {};
    };

    Value visit(VarDecl& t_node) override {
        std::cout << "defining " << t_node.name;
        Symbol* symbol = defineSymbol(t_node.type, t_node.name);
        std::cout << ' ' << symbol << '\n';
        t_node.symbol = symbol;

        return {};
    };
};

} // namespace euclide