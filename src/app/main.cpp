#include <iostream>

#include "interface/engine.h"
#include "interface/panels/node_graph.h"

#include <memory>

int main(){
    butter::Engine engine{1200, 800, "Test"};
    
    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    
    engine.mainLoop();
    return 0;
}