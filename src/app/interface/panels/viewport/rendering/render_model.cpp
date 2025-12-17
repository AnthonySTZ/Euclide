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

    computePoints(t_mesh.pointAttribs);
    computeEdgesAndPrims(t_mesh);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
} // namespace euclide

void RenderModel::computePoints(const AttributeSet& t_pointAttribs) {
    Timer timer{"Points"}; // 18.6ms with omp

    m_numOfPoints = t_pointAttribs.size();
    const auto positions = t_pointAttribs.find("P");
    const auto normals = t_pointAttribs.find("N");
    const auto colors = t_pointAttribs.find("Cd");

    const bool hasPos = isFloat3(positions);
    const bool hasNormals = isFloat3(normals);
    const bool hasCol = isFloat3(colors);

    std::vector<float> fallbackPosX(hasPos ? 0 : m_numOfPoints, 0.0f);
    std::vector<float> fallbackPosY(hasPos ? 0 : m_numOfPoints, 0.0f);
    std::vector<float> fallbackPosZ(hasPos ? 0 : m_numOfPoints, 0.0f);

    std::vector<float> fallbackNormalX(hasNormals ? 0 : m_numOfPoints, 0.0f);
    std::vector<float> fallbackNormalY(hasNormals ? 0 : m_numOfPoints, 0.0f);
    std::vector<float> fallbackNormalZ(hasNormals ? 0 : m_numOfPoints, 0.0f);

    std::vector<float> fallbackColorR(hasCol ? 0 : m_numOfPoints, 1.0f);
    std::vector<float> fallbackColorG(hasCol ? 0 : m_numOfPoints, 1.0f);
    std::vector<float> fallbackColorB(hasCol ? 0 : m_numOfPoints, 1.0f);

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
}

void RenderModel::computeEdgesAndPrims(const Mesh& t_mesh) {
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
        Timer timer{"Prim"}; // 25ms for 1000x1000 grid
        size_t primOffset = 0;
        size_t edgeOffset = 0;
        const Vertex* __restrict vertices = t_mesh.vertices.data();
        for (const auto& prim : t_mesh.primitives) {
            // Edges
            const uint32_t numVertices = prim.numVertices;
            if (numVertices <= 1)
                continue;

            const uint32_t vertexIndex = prim.verticesIndex;
            for (size_t i = 0; i < numVertices; ++i) {
                edges[edgeOffset++] = vertices[vertexIndex + i].refPoint;
                edges[edgeOffset++] = vertices[(i + 1) == numVertices ? vertexIndex : vertexIndex + i + 1].refPoint;
            }

            // Vertices
            if (numVertices <= 2)
                continue;
            const uint32_t v0 = vertices[vertexIndex].refPoint;
            for (size_t i = 1; i + 1 < numVertices; ++i) {
                vertexIndices[primOffset++] = v0;
                vertexIndices[primOffset++] = vertices[vertexIndex + i].refPoint;
                vertexIndices[primOffset++] = vertices[vertexIndex + i + 1].refPoint;
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
}

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