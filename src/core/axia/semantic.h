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
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, Symbol> symbols;

    inline Symbol* lookup(const std::string& name) {
        if (symbols.count(name))
            return &symbols[name];
        return parent ? parent->lookup(name) : nullptr;
    }
};

struct AxiaSemantic : ASTVisitor {
    std::shared_ptr<Scope> current = std::make_shared<Scope>();

    inline void enterScope() { current = std::make_shared<Scope>(Scope{current}); }
    inline void exitScope() { current = current->parent; }
    Symbol* defineSymbol(const std::string& t_type, const std::string& t_name);

    inline virtual Value visit(StringLiteral&) override { return {}; }
    inline virtual Value visit(NumericLiteral&) override { return {}; }
    inline virtual Value visit(BinaryOp& t_node) override {
        t_node.left->accept(*this);
        t_node.right->accept(*this);
        return {};
    }
    inline virtual Value visit(AttributeIdentifier&) override { return {}; }
    inline virtual Value visit(Statement& t_node) override { return t_node.right->accept(*this); }

    Value visit(Identifier& t_node) override;

    inline Value visit(Assignment& t_node) override {
        t_node.identifier->accept(*this);
        t_node.value->accept(*this);
        return {};
    };

    inline Value visit(VarDecl& t_node) override {
        // std::cout << "defining " << t_node.name;
        Symbol* symbol = defineSymbol(t_node.type, t_node.name);
        // std::cout << ' ' << symbol << '\n';
        t_node.symbol = symbol;

        return {};
    };
};

} // namespace euclide