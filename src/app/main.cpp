#include "interface/engine.h"

#include "nodes/primitive/grid.h"
#include "nodes/attributes/attribute_noise.h"
#include "nodes/attributes/attribute_axia.h"

int main() {
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();
    engine.interface().addSpreadsheet();

    engine.mainLoop();
    return 0;
}