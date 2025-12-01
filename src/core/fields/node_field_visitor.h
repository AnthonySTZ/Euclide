#pragma once

#include "utils/types.h"
#include <string>

namespace euclide {

template <typename T> class NodeField;

/// @brief Visitor interface for node fields, following the Visitor design pattern.
///        Allows operations on different types of NodeField without modifying the NodeField classes.
struct NodeFieldVisitor {
    /// @brief Visit a boolean field.
    /// @param name Name of the field.
    /// @param field Reference to the NodeField<bool> object.
    virtual void visit(const std::string&, NodeField<bool>&) = 0;

    /// @brief Visit an integer field.
    virtual void visit(const std::string&, NodeField<int>&) = 0;

    /// @brief Visit a 2-component integer vector field.
    virtual void visit(const std::string&, NodeField<int2>&) = 0;

    /// @brief Visit a float field.
    virtual void visit(const std::string&, NodeField<float>&) = 0;

    /// @brief Visit a 2-component float vector field.
    virtual void visit(const std::string&, NodeField<float2>&) = 0;

    /// @brief Visit a 3-component float vector field.
    virtual void visit(const std::string&, NodeField<float3>&) = 0;

    /// @brief Virtual destructor.
    virtual ~NodeFieldVisitor() = default;
};

} // namespace euclide
