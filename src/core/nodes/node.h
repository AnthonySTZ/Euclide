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
    Observer<uint32_t, uint32_t, uint32_t, uint32_t> onSetInput; // source_id, source_index, dest_id, dest_index
    Observer<uint32_t, uint32_t, uint32_t> onRemoveInput; // source_id, dest_id, dest_index
    Observer<std::shared_ptr<Node>> onDirty;

    Node(const size_t t_nInputs, const size_t t_nOutputs, const std::string& t_name = "Unknown");

    std::shared_ptr<Mesh> cook(const size_t t_index);
    void setInput(const size_t t_index, const std::shared_ptr<Node> &t_sourceNode, const size_t t_sourceIndex = 0);
    void deleteInputConnection(const size_t t_index);
    void deleteOutputConnections(const size_t t_index);
    void clearConnections();
    void setDirty();
    
    void addField(const std::string& t_name, std::shared_ptr<NodeFieldBase> t_field);
    
    std::shared_ptr<NodeConnection> getInputConnection(const size_t t_index);
    std::vector<std::shared_ptr<NodeConnection>> getOutputConnections(const size_t t_index);
    std::string name() const noexcept { return m_name; }
    void setName(const std::string& t_name) noexcept { m_name = t_name; }
    
    int numInputs() const noexcept { return m_inputConnections.size(); }
    int numOutputs() const noexcept { return m_outputConnections.size(); } 
    
    template <typename T>
    std::shared_ptr<T> getField(const std::string& t_fieldName) {
        auto it = m_fields.find(t_fieldName);
        if (it == m_fields.end()) return nullptr;
        
        return std::dynamic_pointer_cast<T>(it->second);
    }
    
    void setId(const uint32_t t_id) { m_id = t_id; }
    uint32_t id() const noexcept { return m_id; }
    
    bool isInInputsHierarchy(const std::shared_ptr<Node> t_node) const noexcept;
    void setIsRender(bool t_isRender) noexcept { m_isRender = t_isRender; }
    bool isRender() const noexcept { return m_isRender; }
private:
    virtual std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) = 0;

    std::string m_name = "Node";

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;

    bool m_isDirty;
    std::vector<std::shared_ptr<Mesh>> m_cachedMesh; // cache every outputs

    uint32_t m_id = 0;

    bool m_isRender = false;
protected:
    std::unordered_map<std::string, std::shared_ptr<NodeFieldBase>> m_fields;
};

}