#pragma once

#include <glad/glad.h>

#include "geometry/mesh.h"
#include <memory>

namespace butter {

struct RenderVertex {
    float position[3];
    float color[3];
    float normal[3];
};

struct Edge { 
    uint32_t a, b;
};
inline Edge makeEdge(uint32_t a, uint32_t b) { return {std::min(a,b), std::max(a,b)}; }

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
    uint32_t m_eboVertex = 0;
    uint32_t m_eboPoints = 0;
    uint32_t m_eboEdges = 0;

    size_t m_numOfPoints = 0;
    size_t m_numOfVertexIndices = 0; // Triangulized mesh
    size_t m_numOfEdgesIndices = 0;


};

}