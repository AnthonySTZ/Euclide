#include "subdivide.h"

#include "smooth_normals.h"

#include <iostream>

namespace euclide {

Subdivide::Subdivide() : Node(1, 1, "Subdivide") {
    auto divisionsField = std::make_shared<NodeField<int>>(1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName : "Divisions",
        min : 0,
        max : 10,
        step : 1,
    });
    addField("divisions", divisionsField);
}

std::shared_ptr<Mesh> Subdivide::compute(const size_t t_index,
                                         const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int divisions = getField<NodeField<int>>("divisions")->getValue();

    const SubdivideSettings settings{.divisions = divisions};

    subdivide(*output, settings);
    SmoothNormals::smoothNormals(*output);

    return output;
}

size_t valence(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges) {
    // Number of edges connected to a point
    size_t n = 1;
    size_t h_primeIdx = t_halfedges[t_halfedges[t_idx].twin].next;
    while (h_primeIdx != t_idx) {
        n++;
        h_primeIdx = t_halfedges[t_halfedges[h_primeIdx].twin].next;
    }
    return n;
}

size_t isOnBorder(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges) {
    if (t_halfedges[t_idx].twin == HalfEdge::NO_TWIN)
        return true;

    size_t h_primeIdx = t_halfedges[t_halfedges[t_idx].twin].next;
    while (h_primeIdx != t_idx) {
        if (t_halfedges[h_primeIdx].twin == HalfEdge::NO_TWIN)
            return true;
        h_primeIdx = t_halfedges[t_halfedges[h_primeIdx].twin].next;
    }

    return false;
}

void Subdivide::subdivide(Mesh& t_mesh, const SubdivideSettings& t_settings) {
    // Base on paper: 'A Halfedge Refinement Rule for Parallel Catmull-Clark Subdivision'
    // `https://onrendering.com/data/papers/catmark/HalfedgeCatmullClark.pdf`
    for (size_t iteration = 0; iteration < t_settings.divisions; ++iteration) {
        const auto& points_d = t_mesh.points;
        const auto& primitives_d = t_mesh.primitives;

        const std::vector<HalfEdge> halfedges_d = t_mesh.computeHalfEdges();
        const uint32_t numOfEdges = getNumberOfEdgesFromHalfedges(halfedges_d);
        const uint32_t numOfPoints = points_d.size();
        const uint32_t numOfPrims = primitives_d.size();

        // Init new Halfedge
        std::vector<HalfEdge> halfedges_d1(halfedges_d.size() * 4);

        // Init new Points
        Points points_d1{};
        points_d1.resize(numOfPoints + numOfPrims + numOfEdges);
        std::fill(std::begin(points_d1.colorR), std::end(points_d1.colorR), 1.0);
        std::fill(std::begin(points_d1.colorG), std::end(points_d1.colorG), 1.0);
        std::fill(std::begin(points_d1.colorB), std::end(points_d1.colorB), 1.0);

        // Halfedge refinement
        halfedgeRefinement(halfedges_d1, halfedges_d, numOfPoints, numOfPrims);

        // Face points
        computeFacePoints(halfedges_d, points_d1, points_d, primitives_d, numOfPoints);

        // Smooth edge points
        smoothEdgePoints(halfedges_d, points_d1, points_d, numOfPoints, numOfPrims);

        // Smooth vertex points
        smoothVertexPoints(halfedges_d, points_d1, points_d, numOfPoints, numOfPrims);

        t_mesh.reconstructFromHalfEdges(halfedges_d1, points_d1);
    }
}

void Subdivide::halfedgeRefinement(std::vector<HalfEdge>& t_halfedges_d1, const std::vector<HalfEdge>& t_halfedges_d,
                                   const uint32_t t_numOfPoints, const uint32_t t_numOfPrims) {
    HalfEdge* __restrict he_d1 = t_halfedges_d1.data();

#pragma omp parallel for
    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const size_t newFaceId = 4 * h;

        he_d1[newFaceId].next = newFaceId + 1;
        he_d1[newFaceId].origin = hd.origin;
        he_d1[newFaceId].face = h;

        he_d1[newFaceId + 1].next = newFaceId + 2;
        he_d1[newFaceId + 1].origin = t_numOfPoints + t_numOfPrims + hd.edge;
        he_d1[newFaceId + 1].face = h;

        he_d1[newFaceId + 2].next = newFaceId + 3;
        he_d1[newFaceId + 2].origin = t_numOfPoints + hd.face;
        he_d1[newFaceId + 2].face = h;

        he_d1[newFaceId + 3].origin = t_numOfPoints + t_numOfPrims + t_halfedges_d[hd.prev].edge;
        he_d1[newFaceId + 3].next = newFaceId;
        he_d1[newFaceId + 3].face = h;
    }
}

void Subdivide::computeFacePoints(const std::vector<HalfEdge>& t_halfedges_d, Points& t_points_d1,
                                  const Points& t_points_d, const std::vector<Primitive>& t_primitives_d,
                                  const uint32_t t_numOfPoints) {
    float* __restrict x1 = t_points_d1.posX.data();
    float* __restrict y1 = t_points_d1.posY.data();
    float* __restrict z1 = t_points_d1.posZ.data();

    const float* __restrict x0 = t_points_d.posX.data();
    const float* __restrict y0 = t_points_d.posY.data();
    const float* __restrict z0 = t_points_d.posZ.data();

    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const float factor = 1.0f / static_cast<float>(t_primitives_d[hd.face].numVertices);
        const uint32_t i = t_numOfPoints + hd.face;
        x1[i] += x0[hd.origin] * factor;
        y1[i] += y0[hd.origin] * factor;
        z1[i] += z0[hd.origin] * factor;
    }
}

void Subdivide::smoothEdgePoints(const std::vector<HalfEdge>& t_halfedges_d, Points& t_points_d1,
                                 const Points& t_points_d, const uint32_t t_numOfPoints, const uint32_t t_numOfPrims) {
    float* __restrict x1 = t_points_d1.posX.data();
    float* __restrict y1 = t_points_d1.posY.data();
    float* __restrict z1 = t_points_d1.posZ.data();

    const float* __restrict x0 = t_points_d.posX.data();
    const float* __restrict y0 = t_points_d.posY.data();
    const float* __restrict z0 = t_points_d.posZ.data();

    for (uint32_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const uint32_t i = t_numOfPoints + hd.face;
        const size_t j = t_numOfPoints + t_numOfPrims + hd.edge;
        if (hd.twin == HalfEdge::NO_TWIN) {
            const uint32_t next = t_halfedges_d[hd.next].origin;
            x1[j] = (x0[hd.origin] + x0[next]) * 0.5f;
            y1[j] = (y0[hd.origin] + y0[next]) * 0.5f;
            z1[j] = (z0[hd.origin] + z0[next]) * 0.5f;
            continue;
        }

        x1[j] += (x0[hd.origin] + x1[i]) * 0.25f;
        y1[j] += (y0[hd.origin] + y1[i]) * 0.25f;
        z1[j] += (z0[hd.origin] + z1[i]) * 0.25f;
    }
}

void Subdivide::smoothVertexPoints(const std::vector<HalfEdge>& t_halfedges_d, Points& t_points_d1,
                                   const Points& t_points_d, const uint32_t t_numOfPoints,
                                   const uint32_t t_numOfPrims) {
    float* __restrict x1 = t_points_d1.posX.data();
    float* __restrict y1 = t_points_d1.posY.data();
    float* __restrict z1 = t_points_d1.posZ.data();

    const float* __restrict x0 = t_points_d.posX.data();
    const float* __restrict y0 = t_points_d.posY.data();
    const float* __restrict z0 = t_points_d.posZ.data();

    const uint32_t edgeCount = static_cast<uint32_t>(t_halfedges_d.size());

    std::vector<uint8_t> onBorder(edgeCount);
    std::vector<uint32_t> valences(edgeCount);
#pragma omp parallel for
    for (uint32_t h = 0; h < edgeCount; ++h) {
        const bool border = isOnBorder(h, t_halfedges_d);
        onBorder[h] = border ? 1 : 0;
        if (!border) {
            valences[h] = valence(h, t_halfedges_d);
        }
    }

    for (uint32_t h = 0; h < edgeCount; ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const uint32_t origin = hd.origin;

        if (onBorder[h]) {
            x1[origin] = x0[origin];
            y1[origin] = y0[origin];
            z1[origin] = z0[origin];
            continue;
        }
        const float n = static_cast<float>(valences[h]);
        const float invN = 1.0f / n;
        const float factor = invN * invN;
        const float n_3 = n - 3;
        const uint32_t i = t_numOfPoints + hd.face;
        const size_t j = t_numOfPoints + t_numOfPrims + hd.edge;
        x1[origin] += (4.0f * x1[j] - x1[i] + n_3 * x0[origin]) * factor;
        y1[origin] += (4.0f * y1[j] - y1[i] + n_3 * y0[origin]) * factor;
        z1[origin] += (4.0f * z1[j] - z1[i] + n_3 * z0[origin]) * factor;
    }
}

} // namespace euclide