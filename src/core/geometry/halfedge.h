#pragma once

namespace butter {

struct HalfEdge {
    uint32_t twin;
    uint32_t next;
    uint32_t origin;
    uint32_t face;
};

}