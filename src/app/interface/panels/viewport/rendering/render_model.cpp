#include "render_model.h"

#include "utils/timer.h"

#include <numeric>

namespace euclide {

RenderModel::RenderModel() {
    initBuffers();
}

RenderModel::~RenderModel() {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, position)); // Position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, color)); // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, normal)); // Normal

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderModel::updateWithMesh(const Mesh& t_mesh) {
    Timer timer{"Build Render Model"}; // 100ms for 1000x1000 grid

    m_numOfPrims = t_mesh.primitives.size();
    glBindVertexArray(m_vao);
    const auto& pointsAttribs = t_mesh.pointAttribs;
    m_numOfPoints = pointsAttribs.size();
    const auto positions = pointsAttribs.find("P");
    const auto normals = pointsAttribs.find("N");
    const auto colors = pointsAttribs.find("Cd");

    std::vector<float> fallbackPosX(m_numOfPoints, 0.0f);
    std::vector<float> fallbackPosY(m_numOfPoints, 0.0f);
    std::vector<float> fallbackPosZ(m_numOfPoints, 0.0f);

    std::vector<float> fallbackNormalX(m_numOfPoints, 0.0f);
    std::vector<float> fallbackNormalY(m_numOfPoints, 0.0f);
    std::vector<float> fallbackNormalZ(m_numOfPoints, 0.0f);

    std::vector<float> fallbackColorR(m_numOfPoints, 1.0f);
    std::vector<float> fallbackColorG(m_numOfPoints, 1.0f);
    std::vector<float> fallbackColorB(m_numOfPoints, 1.0f);

    const bool hasPos = isFloat3(positions);
    const bool hasNormals = isFloat3(normals);
    const bool hasCol = isFloat3(colors);

    const float* points_posX = hasPos ? positions->component<float>(0) : fallbackPosX.data();
    const float* points_posY = hasPos ? positions->component<float>(1) : fallbackPosY.data();
    const float* points_posZ = hasPos ? positions->component<float>(2) : fallbackPosZ.data();

    const float* points_normalX = hasNormals ? normals->component<float>(0) : fallbackNormalX.data();
    const float* points_normalY = hasNormals ? normals->component<float>(1) : fallbackNormalY.data();
    const float* points_normalZ = hasNormals ? normals->component<float>(2) : fallbackNormalZ.data();

    const float* points_colorR = hasCol ? colors->component<float>(0) : fallbackColorR.data();
    const float* points_colorG = hasCol ? colors->component<float>(1) : fallbackColorG.data();
    const float* points_colorB = hasCol ? colors->component<float>(2) : fallbackColorB.data();

    {
        Timer timer{"Points"}; // 11.76 ms 1000x1000 grid -> 11ms with omp

        std::vector<RenderVertex> vertices;
        vertices.resize(m_numOfPoints);
#pragma omp parallel for
        for (size_t i = 0; i < m_numOfPoints; ++i) {
            vertices[i].position[0] = points_posX[i];
            vertices[i].position[1] = points_posY[i];
            vertices[i].position[2] = points_posZ[i];

            vertices[i].color[0] = points_colorR[i];
            vertices[i].color[1] = points_colorG[i];
            vertices[i].color[2] = points_colorB[i];

            vertices[i].normal[0] = points_normalX[i];
            vertices[i].normal[1] = points_normalY[i];
            vertices[i].normal[2] = points_normalZ[i];
        }

        bindVBO(vertices);
    }

    size_t totalTriangles = 0;
    size_t totalEdges = 0;
    for (const auto& prim : t_mesh.primitives) {
        if (prim.numVertices <= 1)
            continue;
        totalEdges += prim.numVertices == 2 ? 1 : prim.numVertices;
        if (prim.numVertices <= 2)
            continue;
        totalTriangles += prim.numVertices - 2;
    }
    std::vector<uint32_t> vertexIndices(totalTriangles * 3);
    std::vector<uint32_t> edges(totalEdges * 2);

    {
        Timer timer{"Prim"}; // 45ms for 1000x1000 grid
        size_t primOffset = 0;
        size_t edgeOffset = 0;
        const auto& vertices = t_mesh.vertices;
        for (const auto& prim : t_mesh.primitives) {
            // Edges
            if (prim.numVertices <= 1)
                continue;
            for (size_t i = 0; i < prim.numVertices; ++i) {
                edges[edgeOffset++] = vertices[prim.verticesIndex + i].refPoint;
                edges[edgeOffset++] = vertices[prim.verticesIndex + (i + 1) % prim.numVertices].refPoint;
            }

            // Vertices
            if (prim.numVertices <= 2)
                continue;
            const uint32_t v0 = vertices[prim.verticesIndex].refPoint;
            for (size_t i = 1; i + 1 < prim.numVertices; ++i) {
                vertexIndices[primOffset++] = v0;
                vertexIndices[primOffset++] = vertices[prim.verticesIndex + i].refPoint;
                vertexIndices[primOffset++] = vertices[prim.verticesIndex + i + 1].refPoint;
            }
        }
    }

    m_numOfVertexIndices = vertexIndices.size();
    bindEBOVertex(vertexIndices);

    {
        std::vector<uint32_t> pointIndices(m_numOfPoints);
        std::iota(pointIndices.begin(), pointIndices.end(), 0); // 0, 1, ..., m_numOfPoints-1
        bindEBOPoints(pointIndices);
    }

    m_numOfEdgesIndices = edges.size();
    bindEBOEdges(edges);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
} // namespace euclide

void RenderModel::bindVBO(const std::vector<RenderVertex>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(RenderVertex), vertices.data(), GL_DYNAMIC_DRAW);
}

void RenderModel::bindEBOVertex(const std::vector<uint32_t>& vertexIndices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(uint32_t), vertexIndices.data(),
                 GL_DYNAMIC_DRAW);
}

void RenderModel::bindEBOPoints(const std::vector<uint32_t>& pointIndices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, pointIndices.size() * sizeof(uint32_t), pointIndices.data(), GL_DYNAMIC_DRAW);
}

void RenderModel::bindEBOEdges(const std::vector<uint32_t>& edges) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfEdgesIndices * sizeof(uint32_t), edges.data(), GL_DYNAMIC_DRAW);
}

void RenderModel::draw() const {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glDrawElements(GL_TRIANGLES, m_numOfVertexIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void RenderModel::drawPoints() const {
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboPoints);
    glDrawElements(GL_POINTS, m_numOfPoints, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void RenderModel::drawEdges() const {
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glDrawElements(GL_LINES, m_numOfEdgesIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

} // namespace euclide