#include <iostream>

#include "interface/engine.h"
#include "interface/panels/node_graph_panel.h"

#include <memory>

int main(){
    butter::Engine engine{1200, 800, "Test"};
    
    engine.interface().addNodeGraph();
    
    engine.mainLoop();
    return 0;
}