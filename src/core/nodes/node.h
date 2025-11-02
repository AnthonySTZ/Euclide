#pragma once

#include "geometry/mesh.h"
#include "node_connection.h"

#include <memory>

namespace butter {

class Node : public std::enable_shared_from_this<Node> {

public:

    Node(const size_t t_nInputs, const size_t t_nOutputs);

    std::shared_ptr<Mesh> cook(const size_t t_index);
    void setInput(const size_t t_sourceIndex, const std::weak_ptr<Node>& t_sourceNode, const size_t t_destIndex = 0);
    void setDirty();

private:
    virtual std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) = 0;

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;

    std::vector<uint8_t> m_isDirty;
    std::vector<std::shared_ptr<Mesh>> m_cachedMesh; // cache every outputs
};

}