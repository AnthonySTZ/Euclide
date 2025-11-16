#pragma once

#include "utils/types.h"
#include <string>

namespace butter {

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