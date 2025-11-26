#pragma once

namespace butter {

struct HalfEdge {
    static constexpr uint32_t NO_TWIN = UINT32_MAX;

    uint32_t next; // Next HalfEdge Index
    uint32_t origin; // Origin vertex Index
    uint32_t face; // Prim Index
    uint32_t twin = NO_TWIN; // Twin HalfEdge Index
};

}