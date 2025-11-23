#include <iostream>

#include "interface/engine.h"
#include "interface/panels/node_graph.h"

int main(){
    butter::Engine engine{1600, 900, "Test"};
    
    engine.interface().addNodeGraph();
    engine.interface().addViewport();
    engine.interface().addParameters();
    
    engine.mainLoop();
    return 0;
}