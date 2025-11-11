#pragma once

#include <glad/glad.h>

#include "py.h"

#include "geometry/mesh.h"
#include <memory>

namespace butter {

struct RenderVertex {
    py::vec3 position;
    py::vec3 color;
    py::vec3 normal;
};

class RenderModel {

public:

    RenderModel();
    ~RenderModel();
    
    void updateWithMesh(std::shared_ptr<Mesh> t_mesh);
    
    void draw();
    void drawPoints();
    void drawEdges();

private:
    void initBuffers();

    uint32_t m_vao = 0;
    uint32_t m_vbo = 0;
    uint32_t m_eboPoints = 0;
    uint32_t m_eboEdges = 0;

    size_t m_numOfVertices = 0;
    size_t m_numOfEdgesIndices = 0;


};

}