#pragma once

#include "panel.h"

namespace butter {

class NodeGraph : public Panel {

public:
    NodeGraph() = default;
    ~NodeGraph() = default;

    void draw() override;

};

}