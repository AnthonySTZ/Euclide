#include "render_model.h"

#include "interface/utils/hash.h"

#include <numeric>
#include <unordered_set>
#include <iostream>

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
    auto t1 = std::chrono::high_resolution_clock::now();


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
    
    std::vector<uint32_t> vertexIndices;
    std::unordered_set<std::pair<uint32_t, uint32_t>, EdgeHash> edges;
    std::vector<uint32_t> edgeIndices;
    for(size_t primId = 0; primId < t_mesh->primitives.size(); ++primId){
        const std::vector<uint32_t> primPointIds = std::move(t_mesh->getPointIndicesOfPrimitive(primId));
        const size_t numOfPointIds = primPointIds.size();
        if (numOfPointIds <= 1) continue;

        // Edges
        for (size_t i = 0; i < numOfPointIds; ++i){
            uint32_t first = primPointIds[i];
            uint32_t second = primPointIds[(i + 1) % numOfPointIds];

            if (first > second) std::swap(first, second);
            auto edge = std::make_pair(first, second);
            if (edges.insert(edge).second){
                edgeIndices.push_back(first);
                edgeIndices.push_back(second);
            }
        }

        if (numOfPointIds <= 2) continue;

        // Vertices
        for (size_t i = 1; i + 1 < numOfPointIds; ++i){
            vertexIndices.push_back(primPointIds[0]);
            vertexIndices.push_back(primPointIds[i]);
            vertexIndices.push_back(primPointIds[i+1]);
        }

    }
    
    m_numOfVertexIndices = vertexIndices.size();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(uint32_t), vertexIndices.data(), GL_DYNAMIC_DRAW);


    std::vector<uint32_t> pointIndices(m_numOfPoints);
    std::iota(pointIndices.begin(), pointIndices.end(), 0); // 0, 1, ..., m_numOfPoints-1
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices.size() * sizeof(uint32_t), pointIndices.data(), GL_DYNAMIC_DRAW);

    m_numOfEdgesIndices = edgeIndices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(uint32_t), edgeIndices.data(), GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    auto t2 = std::chrono::high_resolution_clock::now();
    auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_int.count() << "ms\n";
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