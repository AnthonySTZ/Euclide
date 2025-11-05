#include <iostream>

#include "interface/engine.h"
#include "interface/panels/node_graph_panel.h"

#include <memory>

int main(){
    butter::Engine engine{1200, 800, "Test"};
    
    std::shared_ptr<butter::NodeGraph> nodeGraph = std::make_shared<butter::NodeGraph>();
    engine.interface().addPanel(nodeGraph);
    
    engine.mainLoop();
    return 0;
}