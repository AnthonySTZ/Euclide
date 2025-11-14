#include "render_model.h"

#include "interface/utils/hash.h"

#include <numeric>
#include <iostream>
#include <algorithm>

#include <chrono>

namespace butter {
    
RenderModel::RenderModel()
{
    initBuffers();
}    

RenderModel::~RenderModel()
{
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_eboVertex != 0) {
        glDeleteBuffers(1, &m_eboVertex);
        m_eboVertex = 0;
    }
    if (m_eboPoints != 0) {
        glDeleteBuffers(1, &m_eboPoints);
        m_eboPoints = 0;
    }
    if (m_eboEdges != 0) {
        glDeleteBuffers(1, &m_eboEdges);
        m_eboEdges = 0;
    }
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

void RenderModel::initBuffers() {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_eboVertex);
    glGenBuffers(1, &m_eboPoints);
    glGenBuffers(1, &m_eboEdges);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void *)offsetof(RenderVertex, position)); // Position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void *)offsetof(RenderVertex, color)); // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex), (void *)offsetof(RenderVertex, normal)); // Normal
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderModel::updateWithMesh(std::shared_ptr<Mesh> t_mesh)
{
    const Points& points = t_mesh->points;
    m_numOfPoints = points.size();
    
    std::vector<RenderVertex> vertices;
    vertices.resize(m_numOfPoints);
    for (size_t i = 0; i < m_numOfPoints; ++i) {
        vertices[i].position[0] = points.posX[i];
        vertices[i].position[1] = points.posY[i];
        vertices[i].position[2] = points.posZ[i];

        vertices[i].color[0] = points.colorR[i];
        vertices[i].color[1] = points.colorG[i];
        vertices[i].color[2] = points.colorB[i];

        vertices[i].normal[0] = points.normalX[i];
        vertices[i].normal[1] = points.normalY[i];
        vertices[i].normal[2] = points.normalZ[i];
    }
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(RenderVertex), vertices.data(), GL_DYNAMIC_DRAW);
    
    std::vector<uint32_t> vertexIndices;
    std::vector<Edge> edges;
    for(size_t primId = 0; primId < t_mesh->primitives.size(); ++primId){
        const std::vector<uint32_t> primPointIds = std::move(t_mesh->getPointIndicesOfPrimitive(primId));
        const size_t numOfPointIds = primPointIds.size();
        if (numOfPointIds <= 1) continue;

        // Edges
        for (size_t i = 0; i < numOfPointIds; ++i){
            uint32_t first = primPointIds[i];
            uint32_t second = primPointIds[(i + 1) % numOfPointIds];

            edges.push_back(makeEdge(first, second));
        }

        if (numOfPointIds <= 3) continue;

        // Vertices
        for (size_t i = 1; i + 1 < numOfPointIds; ++i){
            vertexIndices.push_back(primPointIds[0]);
            vertexIndices.push_back(primPointIds[i]);
            vertexIndices.push_back(primPointIds[i+1]);
        }

    }
    std::sort(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2){ 
        return e1.a < e2.a || (e1.a==e2.a && e1.b < e2.b); 
    });
    edges.erase(std::unique(edges.begin(), edges.end(), [](const Edge &e1, const Edge &e2){
        return e1.a==e2.a && e1.b==e2.b;
    }), edges.end());
    
    m_numOfVertexIndices = vertexIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(uint32_t), vertexIndices.data(), GL_DYNAMIC_DRAW);

    std::vector<uint32_t> pointIndices(m_numOfPoints);
    std::iota(pointIndices.begin(), pointIndices.end(), 0); // 0, 1, ..., m_numOfPoints-1
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices.size() * sizeof(uint32_t), pointIndices.data(), GL_DYNAMIC_DRAW);

    m_numOfEdgesIndices = edges.size() * 2;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfEdgesIndices * sizeof(uint32_t), edges.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


void RenderModel::draw() {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glDrawElements(GL_TRIANGLES, m_numOfVertexIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void RenderModel::drawPoints() {

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
	glDrawElements(
        GL_POINTS,
        m_numOfPoints,
        GL_UNSIGNED_INT,
        0
    );
    
	glBindVertexArray(0);
    
    
}
void RenderModel::drawEdges() {
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
	glDrawElements(
        GL_LINES,
        m_numOfEdgesIndices,
        GL_UNSIGNED_INT,
        0
    );
    
	glBindVertexArray(0);

}

}