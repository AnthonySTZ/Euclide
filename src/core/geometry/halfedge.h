#pragma once

namespace butter {

struct HalfEdge {
    static constexpr uint32_t NO_TWIN = UINT32_MAX;

    uint32_t next; // Next HalfEdge Index
    uint32_t prev; // Previous HalfEdge Index
    uint32_t origin; // Origin vertex Index
    uint32_t face; // Prim Index
    uint32_t twin = NO_TWIN; // Twin HalfEdge Index
};

struct Edge {
    size_t key;
    size_t idx; 
};

inline void radixSortEdges(std::vector<Edge>& edges) {

    constexpr int BITS_PER_PASS = 16;
    constexpr int NUM_BUCKETS = 1 << BITS_PER_PASS;
    constexpr uint64_t MASK = NUM_BUCKETS - 1;

    const size_t countEdges = edges.size();
    std::vector<Edge> temp(countEdges);
    std::vector<size_t> bucketCount(NUM_BUCKETS);

    for (int bitShift = 0; bitShift < 64; bitShift += BITS_PER_PASS)
    {
        std::fill(bucketCount.begin(), bucketCount.end(), 0);
        for (const Edge& e : edges) {
            uint16_t bucket = (e.key >> bitShift) & MASK;
            bucketCount[bucket]++;
        }

        size_t offset = 0;
        for (size_t& count : bucketCount) {
            size_t oldCount = count;
            count = offset;
            offset += oldCount;
        }

        for (const Edge& e : edges) {
            uint16_t bucket = (e.key >> bitShift) & MASK;
            temp[bucketCount[bucket]++] = e;
        }

        edges.swap(temp);
    }
}

}