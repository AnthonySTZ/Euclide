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

class RenderModel {

public:

    RenderModel();
    ~RenderModel();
    
    void updateWithMesh(const Mesh& t_mesh);

    void draw() const;
    void drawPoints() const;
    void drawEdges() const;

    void tooglePrimitives() noexcept { showPrimitives = !showPrimitives; }
    void toogleWireframe() noexcept { showWireframe = !showWireframe; }
    void tooglePoints() noexcept { showPoints = !showPoints; }

    bool showPrimitives = true;
    bool showWireframe = true;
    bool showPoints = false;
    
private:
    void initBuffers();
    void bindVBO(const std::vector<RenderVertex> &vertices);

    void bindEBOVertex(const std::vector<uint32_t> &vertexIndices);

    void bindEBOPoints(const std::vector<uint32_t> &pointIndices);

    void bindEBOEdges(const std::vector<uint32_t> &edges);

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