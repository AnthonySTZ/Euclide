#pragma once

#include "mesh.h"
#include "node_connection.h"

#include <memory>

namespace butter {

class Node {

public:

    Node(const size_t t_nInputs, const size_t t_nOutputs)
    : m_nInputs{t_nInputs},
      m_nOutputs{t_nOutputs},
      m_inputConnections(t_nInputs),
      m_outputConnections(t_nOutputs),
      m_cachedMesh(t_nOutputs)
    {
        m_isDirty.resize(t_nOutputs, true);
    }

    std::shared_ptr<Mesh> cook(const size_t t_index) {
        if (!m_isDirty[t_index]) return m_cachedMesh[t_index];

        std::vector<std::shared_ptr<Mesh>> inputMeshes;
        for (auto& conn: m_inputConnections){
            if (conn == nullptr) {
                inputMeshes.push_back(nullptr);
                continue;
            }
            if (auto sourceNode = conn->sourceNode.lock()) {
                inputMeshes.push_back(sourceNode->cook(t_index));
            }
        }

        m_cachedMesh[t_index] = compute(t_index, inputMeshes);
        m_isDirty[t_index] = false;
        return m_cachedMesh[t_index];
    }

private:
    virtual std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) = 0;
    
    size_t m_nInputs = 0;
    size_t m_nOutputs = 0;

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;

    std::vector<uint8_t> m_isDirty;
    std::vector<std::shared_ptr<Mesh>> m_cachedMesh;
};

}