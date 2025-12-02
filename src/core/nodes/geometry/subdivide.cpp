#include "subdivide.h"

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
        // TODO: Check multi threading, simd, etc..
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
        std::fill(std::begin(points_d1.normalX), std::end(points_d1.normalX), 0.0);
        std::fill(std::begin(points_d1.normalY), std::end(points_d1.normalY), 1.0);
        std::fill(std::begin(points_d1.normalZ), std::end(points_d1.normalZ), 0.0);

        // Halfedge refinement
        halfedgeRefinement(halfedges_d1, halfedges_d, numOfPoints, numOfPrims);

        // Face points
        computeFacePoints(halfedges_d, points_d1, points_d, primitives_d, numOfPoints);

        // Smooth edge points
        for (uint32_t h = 0; h < halfedges_d.size(); ++h) {
            const HalfEdge& hd = halfedges_d[h];
            const uint32_t i = numOfPoints + hd.face;
            const size_t j = numOfPoints + numOfPrims + hd.edge;
            if (hd.twin == HalfEdge::NO_TWIN) {
                const uint32_t next = halfedges_d[hd.next].origin;
                points_d1.posX[j] = (points_d.posX[hd.origin] + points_d.posX[next]) * 0.5f;
                points_d1.posY[j] = (points_d.posY[hd.origin] + points_d.posY[next]) * 0.5f;
                points_d1.posZ[j] = (points_d.posZ[hd.origin] + points_d.posZ[next]) * 0.5f;
                continue;
            }

            points_d1.posX[j] += (points_d.posX[hd.origin] + points_d1.posX[i]) * 0.25f;
            points_d1.posY[j] += (points_d.posY[hd.origin] + points_d1.posY[i]) * 0.25f;
            points_d1.posZ[j] += (points_d.posZ[hd.origin] + points_d1.posZ[i]) * 0.25f;
        }

        // Smooth vertex points
        for (uint32_t h = 0; h < halfedges_d.size(); ++h) {
            const HalfEdge& hd = halfedges_d[h];
            if (isOnBorder(h, halfedges_d)) {
                points_d1.posX[hd.origin] = points_d.posX[hd.origin];
                points_d1.posY[hd.origin] = points_d.posY[hd.origin];
                points_d1.posZ[hd.origin] = points_d.posZ[hd.origin];
                continue;
            }
            const float n = static_cast<float>(valence(h, halfedges_d));
            const float factor = 1.0f / (n * n);
            const float n_3 = n - 3;
            const uint32_t i = numOfPoints + hd.face;
            const size_t j = numOfPoints + numOfPrims + hd.edge;
            points_d1.posX[hd.origin] +=
                (4.0f * points_d1.posX[j] - points_d1.posX[i] + n_3 * points_d.posX[hd.origin]) * factor;
            points_d1.posY[hd.origin] +=
                (4.0f * points_d1.posY[j] - points_d1.posY[i] + n_3 * points_d.posY[hd.origin]) * factor;
            points_d1.posZ[hd.origin] +=
                (4.0f * points_d1.posZ[j] - points_d1.posZ[i] + n_3 * points_d.posZ[hd.origin]) * factor;
        }

        t_mesh.reconstructFromHalfEdges(halfedges_d1, points_d1);
    }
}

void Subdivide::halfedgeRefinement(std::vector<HalfEdge>& t_halfedges_d1, const std::vector<HalfEdge>& t_halfedges_d,
                                   const uint32_t t_numOfPoints, const uint32_t t_numOfPrims) {
    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const size_t newFaceId = 4 * h;

        t_halfedges_d1[newFaceId].next = newFaceId + 1;
        t_halfedges_d1[newFaceId + 1].next = newFaceId + 2;
        t_halfedges_d1[newFaceId + 2].next = newFaceId + 3;
        t_halfedges_d1[newFaceId + 3].next = newFaceId;

        t_halfedges_d1[newFaceId].origin = hd.origin;
        t_halfedges_d1[newFaceId + 1].origin = t_numOfPoints + t_numOfPrims + hd.edge;
        t_halfedges_d1[newFaceId + 2].origin = t_numOfPoints + hd.face;
        t_halfedges_d1[newFaceId + 3].origin = t_numOfPoints + t_numOfPrims + t_halfedges_d[hd.prev].edge;

        t_halfedges_d1[newFaceId].face = h;
        t_halfedges_d1[newFaceId + 1].face = h;
        t_halfedges_d1[newFaceId + 2].face = h;
        t_halfedges_d1[newFaceId + 3].face = h;
    }
}

void Subdivide::computeFacePoints(const std::vector<HalfEdge>& t_halfedges_d, Points& t_points_d1,
                                  const Points& t_points_d, const std::vector<Primitive>& t_primitives_d,
                                  const uint32_t t_numOfPoints) {
    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const float factor = 1.0f / static_cast<float>(t_primitives_d[hd.face].numVertices);
        const uint32_t i = t_numOfPoints + hd.face;
        t_points_d1.posX[i] += t_points_d.posX[hd.origin] * factor;
        t_points_d1.posY[i] += t_points_d.posY[hd.origin] * factor;
        t_points_d1.posZ[i] += t_points_d.posZ[hd.origin] * factor;
    }
}

} // namespace euclide