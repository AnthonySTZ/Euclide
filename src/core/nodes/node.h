#pragma once

#include "geometry/mesh.h"
#include "node_connection.h"
#include "fields/node_field.h"

#include <memory>
#include <string>

#include <unordered_map>

namespace butter {

class Node : public std::enable_shared_from_this<Node> {

public:
    Node(const size_t t_nInputs, const size_t t_nOutputs, const std::string& t_name = "Unknown");

    std::shared_ptr<Mesh> cook(const size_t t_index);
    void setInput(const size_t t_index, const std::weak_ptr<Node> &t_sourceNode, const size_t t_sourceIndex = 0);
    void deleteInputConnection(const size_t t_index);
    void setDirty();
    std::shared_ptr<NodeConnection> getInputConnection(const size_t t_index);
    std::vector<std::shared_ptr<NodeConnection>> getOutputConnections(const size_t t_index);
    std::string name() const noexcept { return m_name; }
    void setName(const std::string& t_name) noexcept { m_name = t_name; }

    template <typename T>
    std::shared_ptr<T> getField(const std::string& t_fieldName) {
        auto it = m_fields.find(t_fieldName);
        if (it == m_fields.end()) return nullptr;

        return std::dynamic_pointer_cast<T>(it->second);
    }

private:
    virtual std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) = 0;

    std::string m_name = "Node";

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;

    std::vector<uint8_t> m_isDirty;
    std::vector<std::shared_ptr<Mesh>> m_cachedMesh; // cache every outputs

protected:
    std::unordered_map<std::string, std::shared_ptr<NodeFieldBase>> m_fields;
};

}