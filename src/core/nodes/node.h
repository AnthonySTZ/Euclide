#pragma once

#include "geometry/mesh.h"
#include "node_connection.h"
#include "fields/node_field.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace euclide {

using FieldEntry = std::pair<std::string, std::shared_ptr<NodeFieldBase>>;

/// @brief Represents a computational node in the node graph.
///
/// Nodes can have multiple inputs and outputs, store fields, cache meshes, and propagate "dirty" states.
class Node : public std::enable_shared_from_this<Node> {
  public:
    /// @brief Fired when an input connection is set.
    /// @param source_id ID of the source node.
    /// @param source_index Output index of the source node.
    /// @param dest_id ID of this node.
    /// @param dest_index Input index of this node.
    Observer<uint32_t, uint32_t, uint32_t, uint32_t> onSetInput;

    /// @brief Fired when an input connection is removed.
    /// @param source_id ID of the source node.
    /// @param dest_id ID of this node.
    /// @param dest_index Input index of this node.
    Observer<uint32_t, uint32_t, uint32_t> onRemoveInput;

    /// @brief Fired when the node is marked dirty.
    Observer<std::shared_ptr<Node>> onDirty;

    /// @brief Constructs a Node.
    /// @param t_nInputs Number of input slots.
    /// @param t_nOutputs Number of output slots.
    /// @param t_name Name of the node.
    Node(const size_t t_nInputs, const size_t t_nOutputs, const std::string& t_name = "Unknown");

    /// @brief Computes or returns the cached output mesh at a given output index.
    /// @param t_index Output index.
    /// @return Shared pointer to the output mesh.
    std::shared_ptr<Mesh> cook(const size_t t_index);

    /// @brief Sets an input connection from another node.
    /// @param t_index Input index of this node.
    /// @param t_sourceNode Source node.
    /// @param t_sourceIndex Output index of the source node.
    void setInput(const size_t t_index, const std::shared_ptr<Node>& t_sourceNode, const size_t t_sourceIndex = 0);

    /// @brief Removes an input connection at a specific index.
    /// @param t_index Input index to remove.
    void deleteInputConnection(const size_t t_index);

    /// @brief Removes all output connections at a given output index.
    /// @param t_index Output index to clear.
    void deleteOutputConnections(const size_t t_index);

    /// @brief Clears all input and output connections.
    void clearConnections();

    /// @brief Marks the node as dirty and propagates to connected nodes.
    void setDirty();

    /// @brief Adds a field to the node.
    /// @param t_name Name of the field.
    /// @param t_field Shared pointer to the field.
    void addField(const std::string& t_name, std::shared_ptr<NodeFieldBase> t_field);

    /// @brief Gets an input connection.
    /// @param t_index Input index.
    /// @return Shared pointer to the connection.
    [[nodiscard]] std::shared_ptr<NodeConnection> getInputConnection(const size_t t_index) const;

    /// @brief Gets all output connections for a specific output index.
    /// @param t_index Output index.
    /// @return Vector of shared pointers to connections.
    [[nodiscard]] std::vector<std::shared_ptr<NodeConnection>> getOutputConnections(const size_t t_index) const;

    /// @brief Returns the node's name.
    [[nodiscard]] std::string name() const noexcept { return m_name; }

    /// @brief Sets the node's name.
    void setName(const std::string& t_name) noexcept { m_name = t_name; }

    /// @brief Returns the number of input slots.
    [[nodiscard]] int numInputs() const noexcept { return m_inputConnections.size(); }

    /// @brief Returns the number of output slots.
    [[nodiscard]] int numOutputs() const noexcept { return m_outputConnections.size(); }

    /// @brief Returns a field cast to the given type.
    /// @tparam T Expected field type.
    /// @param t_fieldName Name of the field.
    /// @return Shared pointer to the field of type T, or nullptr if not found.
    template <typename T> [[nodiscard]] std::shared_ptr<T> getField(const std::string& t_fieldName) const {
        auto it = m_fields.find(t_fieldName);
        if (it == m_fields.end())
            return nullptr;
        return std::dynamic_pointer_cast<T>(it->second);
    }

    /// @brief Returns all fields in order of addition.
    [[nodiscard]] std::vector<FieldEntry> fields() const noexcept { return m_fieldsOrder; }

    /// @brief Sets the node's unique ID.
    void setId(const uint32_t t_id) { m_id = t_id; }

    /// @brief Returns the node's unique ID.
    [[nodiscard]] uint32_t id() const noexcept { return m_id; }

    /// @brief Checks recursively if the node is present in input hierarchy.
    /// @param t_node Node to check.
    /// @return True if t_node is in inputs hierarchy, false otherwise.
    [[nodiscard]] bool isInInputsHierarchy(const std::shared_ptr<Node> t_node) const noexcept;

    /// @brief Sets whether this node is used for rendering.
    void setIsRender(bool t_isRender) noexcept { m_isRender = t_isRender; }

    /// @brief Returns whether this node is used for rendering.
    [[nodiscard]] bool isRender() const noexcept { return m_isRender; }

  private:
    /// @brief Computes the output mesh given input meshes. Must be overridden in derived nodes.
    virtual std::shared_ptr<Mesh> compute(const size_t t_index,
                                          const std::vector<std::shared_ptr<Mesh>>& t_inputs) const = 0;

    std::string m_name = "Node";

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;

    bool m_isDirty;
    std::vector<std::shared_ptr<Mesh>> m_cachedMesh;

    uint32_t m_id = 0;

    bool m_isRender = false;

  protected:
    std::unordered_map<std::string, std::shared_ptr<NodeFieldBase>> m_fields;
    std::vector<FieldEntry> m_fieldsOrder;
};

} // namespace euclide
