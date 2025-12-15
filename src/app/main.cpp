#include "interface/engine.h"

int main() {
    euclide::Engine engine{1600, 900, "Test"};

    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();
    engine.interface().addSpreadsheet();

    engine.mainLoop();
    return 0;
}