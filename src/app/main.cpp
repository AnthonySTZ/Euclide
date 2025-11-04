#include <iostream>

#include "interface/engine.h"

#include <memory>

int main(){
    butter::Engine engine{1200, 800, "Test"};
    engine.mainLoop();
    return 0;
}