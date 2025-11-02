#pragma once

#include "mesh.h"
#include "node_connection.h"

#include <memory>

namespace butter {

class Node {

public:
    virtual std::shared_ptr<Mesh> compute(const std::shared_ptr<Mesh>& t_input) = 0;

    Node(const size_t t_nInputs, const size_t t_nOutputs)
        : m_nInputs{t_nInputs}, m_nOutputs{t_nOutputs},
          m_inputConnections(t_nInputs), m_outputConnections(t_nOutputs) 
    {}

private:
    size_t m_nInputs = 0;
    size_t m_nOutputs = 0;

    std::vector<std::shared_ptr<NodeConnection>> m_inputConnections;
    std::vector<std::vector<std::shared_ptr<NodeConnection>>> m_outputConnections;
};

}