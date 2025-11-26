#pragma once

namespace butter {

struct HalfEdge {
        static constexpr uint32_t NO_TWIN = UINT32_MAX;
        
        uint32_t next;
        uint32_t origin;
        uint32_t face;
        uint32_t twin = NO_TWIN;
};

}