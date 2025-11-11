#include "render_model.h"

#include <numeric>
#include <set>
#include <iostream>
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
    vertices.reserve(m_numOfPoints);
    for (size_t i = 0; i < m_numOfPoints; ++i) {
        vertices.emplace_back(
            RenderVertex{
                {points.posX[i], points.posY[i], points.posZ[i]},
                {points.colorR[i], points.colorG[i], points.colorB[i]},
                {points.normalX[i], points.normalY[i], points.normalZ[i]},
            }
        );
    }
    
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(RenderVertex), vertices.data(), GL_DYNAMIC_DRAW);
    
    const std::vector<Vertex>& meshVertices = t_mesh->vertices;

    std::vector<uint32_t> vertexIndices;
    for(const Primitive& prim : t_mesh->primitives){
        const std::vector<uint32_t>& primVertices = prim.vertices;
        const size_t numOfPrimVertices = primVertices.size();
        if (numOfPrimVertices <= 2) continue;

        for (size_t i = 1; i + 1 < numOfPrimVertices; ++i){
            const uint32_t first = meshVertices[primVertices[0]].refPoint;
            const uint32_t second = meshVertices[primVertices[i]].refPoint;
            const uint32_t third = meshVertices[primVertices[i + 1]].refPoint;
            vertexIndices.push_back(first);
            vertexIndices.push_back(second);
            vertexIndices.push_back(third);
        }

    }
    m_numOfVertexIndices = vertexIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(uint32_t), vertexIndices.data(), GL_DYNAMIC_DRAW);


    std::vector<uint32_t> pointIndices(m_numOfPoints);
    std::iota(pointIndices.begin(), pointIndices.end(), 0); // 0, 1, ..., m_numOfPoints-1
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices.size() * sizeof(uint32_t), pointIndices.data(), GL_DYNAMIC_DRAW);


    std::set<std::pair<uint32_t, uint32_t>> edges;
    std::vector<uint32_t> edgeIndices;
    
    for(const Primitive& prim : t_mesh->primitives){
        const std::vector<uint32_t>& primVertices = prim.vertices;
        const size_t numOfPrimVertices = primVertices.size();
        if (numOfPrimVertices <= 1) continue;

        for (size_t i = 0; i < numOfPrimVertices; ++i){
            const uint32_t first = meshVertices[primVertices[i]].refPoint;
            const uint32_t second = meshVertices[primVertices[(i + 1) % numOfPrimVertices]].refPoint;

            if (edges.find({first, second}) != edges.end() || edges.find({second, first}) != edges.end()) continue;
            edgeIndices.push_back(first);
            edgeIndices.push_back(second);
            edges.insert({first, second});
        }

    }

    m_numOfEdgesIndices = edgeIndices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(uint32_t), edgeIndices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


void RenderModel::draw() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_numOfVertexIndices, GL_UNSIGNED_INT, nullptr);
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