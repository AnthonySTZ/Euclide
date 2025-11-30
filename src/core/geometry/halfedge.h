#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>
#include <algorithm>

namespace butter {

/// @brief Represents a half-edge in a half-edge mesh structure.
struct HalfEdge {
    static constexpr uint32_t NO_TWIN = UINT32_MAX; ///< Sentinel value for no twin edge.

    uint32_t next;           ///< Index of the next half-edge in the face loop.
    uint32_t prev;           ///< Index of the previous half-edge in the face loop.
    uint32_t edge;           ///< Virtual unique edge ID.
    uint32_t origin;         ///< Index of the origin vertex.
    uint32_t face;           ///< Index of the face (primitive) this half-edge belongs to.
    uint32_t twin = NO_TWIN; ///< Index of the twin half-edge, NO_TWIN if none.
};

/// @brief Represents an edge used for sorting / processing.
struct Edge {
    size_t key; ///< Key used for sorting.
    size_t idx; ///< Original index in the array.
};

/// @brief Sorts edges in-place using a radix sort algorithm (64-bit keys).
/// @param edges Vector of Edge objects to be sorted.
inline void radixSortEdges(std::vector<Edge>& edges) {
    constexpr int BITS_PER_PASS = 16;               ///< Number of bits processed per pass.
    constexpr int NUM_BUCKETS = 1 << BITS_PER_PASS; ///< Number of buckets per pass.
    constexpr uint64_t MASK = NUM_BUCKETS - 1;      ///< Mask for extracting bits.

    const size_t countEdges = edges.size();
    std::vector<Edge> temp(countEdges);
    std::vector<size_t> bucketCount(NUM_BUCKETS);

    for (int bitShift = 0; bitShift < 64; bitShift += BITS_PER_PASS) {
        std::fill(bucketCount.begin(), bucketCount.end(), 0);

        // Count edges per bucket.
        for (const Edge& e : edges) {
            uint16_t bucket = (e.key >> bitShift) & MASK;
            bucketCount[bucket]++;
        }

        // Compute bucket offsets.
        size_t offset = 0;
        for (size_t& count : bucketCount) {
            size_t oldCount = count;
            count = offset;
            offset += oldCount;
        }

        // Place edges into buckets.
        for (const Edge& e : edges) {
            uint16_t bucket = (e.key >> bitShift) & MASK;
            temp[bucketCount[bucket]++] = e;
        }

        edges.swap(temp);
    }
}

} // namespace butter
