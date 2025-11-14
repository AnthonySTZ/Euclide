#pragma once

#include <array>
#include <string>

namespace butter {

using float3 = std::array<float, 3>;

    
template <typename T>
class NodeField;

struct NodeFieldVisitor {
    virtual void visit(const std::string&, NodeField<int>&) = 0;
    virtual void visit(const std::string&, NodeField<float>&) = 0;
    virtual void visit(const std::string&, NodeField<float3>&) = 0;
    
    virtual ~NodeFieldVisitor() = default;
};

}