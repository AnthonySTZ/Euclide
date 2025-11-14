#pragma once

#include <array>
#include <string>

namespace butter {

using int2 = std::array<int, 2>;

using float2 = std::array<float, 2>;
using float3 = std::array<float, 3>;

    
template <typename T>
class NodeField;

struct NodeFieldVisitor {
    virtual void visit(const std::string&, NodeField<int>&) = 0;
    virtual void visit(const std::string&, NodeField<int2>&) = 0
    ;
    virtual void visit(const std::string&, NodeField<float>&) = 0;
    virtual void visit(const std::string&, NodeField<float2>&) = 0;
    virtual void visit(const std::string&, NodeField<float3>&) = 0;
    
    virtual ~NodeFieldVisitor() = default;
};

}