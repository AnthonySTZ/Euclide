#include "mesh.h"

#include "utils/timer.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

#include <algorithm>
#include "utils/hash.h"

namespace euclide {

// float3 Mesh::center() const {
//     float3 sumPos{0.0, 0.0, 0.0};
//     const size_t numPoints = points.size();
//     if (numPoints == 0)
//         return sumPos;

//     size_t i = 0;

// #ifdef USE_SIMD
//     if (numPoints >= 8) {
//         __m256 __sumX = _mm256_setzero_ps();
//         __m256 __sumY = _mm256_setzero_ps();
//         __m256 __sumZ = _mm256_setzero_ps();

//         for (; i + 8 <= numPoints; i += 8) {
//             __m256 __posX = _mm256_load_ps(&points.posX[i]);
//             __m256 __posY = _mm256_load_ps(&points.posY[i]);
//             __m256 __posZ = _mm256_load_ps(&points.posZ[i]);

//             __sumX = _mm256_add_ps(__sumX, __posX);
//             __sumY = _mm256_add_ps(__sumY, __posY);
//             __sumZ = _mm256_add_ps(__sumZ, __posZ);
//         }

//         sumPos[0] = _mm256_hsum_ps(__sumX);
//         sumPos[1] = _mm256_hsum_ps(__sumY);
//         sumPos[2] = _mm256_hsum_ps(__sumZ);
//     }
// #endif

//     for (; i < numPoints; ++i) {
//         sumPos[0] += points.posX[i];
//         sumPos[1] += points.posY[i];
//         sumPos[2] += points.posZ[i];
//     }

//     const float divisor = static_cast<float>(numPoints);

//     sumPos[0] /= divisor;
//     sumPos[1] /= divisor;
//     sumPos[2] /= divisor;

//     return sumPos;
// }

// std::vector<HalfEdge> Mesh::computeHalfEdges() const {
//     size_t totalHalfEdges = 0;
//     for (const auto& prim : primitives) {
//         if (prim.numVertices > 2) {
//             totalHalfEdges += prim.numVertices;
//         }
//     }
//     std::vector<HalfEdge> halfEdges(totalHalfEdges);

//     std::vector<Edge> edges(totalHalfEdges);

//     uint32_t halfEdgeIdx = 0;
//     for (uint32_t primIdx = 0; primIdx < primitives.size(); ++primIdx) {
//         const Primitive& prim = primitives[primIdx];
//         if (prim.numVertices <= 2)
//             continue;

//         const uint32_t base = prim.verticesIndex;
//         const uint32_t count = prim.numVertices;

//         for (size_t i = 0; i < count; ++i) {
//             const uint32_t origin = vertices[base + i].refPoint;
//             const uint32_t next = vertices[base + (i + 1) % count].refPoint;

//             edges[halfEdgeIdx] = Edge{hash(std::min(origin, next), std::max(origin, next)), halfEdgeIdx};

//             halfEdges[halfEdgeIdx++] = HalfEdge{
//                 .next =
//                     (i + 1 < count) ? halfEdgeIdx : halfEdgeIdx - count, // halfEdgeIdx + 1 but it incremented before
//                 .prev = (i == 0) ? halfEdgeIdx + count - 2 : halfEdgeIdx - 2,
//                 .origin = origin,
//                 .face = primIdx,
//             };
//         }
//     }

//     if (totalHalfEdges > 10000) {
//         radixSortEdges(edges);
//     } else {
//         std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.key < b.key; });
//     }

//     size_t edgeIdx = 0;
//     for (size_t i = 0; i + 1 < edges.size(); ++i) {
//         const auto& a = edges[i];
//         const auto& b = edges[i + 1];
//         auto& halfedge_1 = halfEdges[a.idx];
//         auto& halfedge_2 = halfEdges[b.idx];
//         halfedge_1.edge = edgeIdx;
//         edgeIdx++;
//         halfedge_2.edge = edgeIdx;
//         if (a.key != b.key)
//             continue;

//         if (halfedge_1.origin == halfEdges[halfedge_2.next].origin &&
//             halfedge_2.origin == halfEdges[halfedge_1.next].origin) {
//             halfedge_1.twin = b.idx;
//             halfedge_2.twin = a.idx;
//             halfedge_2.edge = halfedge_1.edge;
//             i++;
//         }
//     }

//     return halfEdges;
// }

// void Mesh::reconstructFromHalfEdges(const std::vector<HalfEdge>& t_halfedges, const Points& t_points) {
//     points = t_points;
//     primitives.clear();
//     vertices.clear();

//     size_t numOfPrim = 0;
//     for (const auto& hf : t_halfedges) {
//         if (hf.face > numOfPrim)
//             numOfPrim = hf.face;
//     }
//     primitives.resize(numOfPrim + 1);

//     for (const auto& hf : t_halfedges) {
//         primitives[hf.face].numVertices++;
//     }

//     for (size_t i = 0; i < t_halfedges.size(); ++i) {
//         const auto& hf = t_halfedges[i];
//         auto& prim = primitives[hf.face];
//         if (prim.verticesIndex != UINT32_MAX)
//             continue;

//         prim.verticesIndex = vertices.size();
//         vertices.emplace_back(Vertex{hf.origin});

//         uint32_t next = hf.next;
//         uint32_t numVertices = 1;
//         while (next != i) {
//             vertices.emplace_back(Vertex{t_halfedges[next].origin});
//             next = t_halfedges[next].next;
//             numVertices++;
//         }
//         prim.numVertices = numVertices;
//     }
// }

} // namespace euclide