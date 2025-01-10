#include "Engine.hpp"
#include <iostream>

int main() {
    Engine engine;
    
    if (!engine.Initialize()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    engine.Run();
    return 0;
} 