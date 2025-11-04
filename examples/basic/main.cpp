/**
 * Basic Example
 * Demonstrates basic engine initialization and shutdown
 */

#include <OpenGameDevEngine.h>
#include <iostream>

int main() {
    std::cout << "OpenGameDevEngine Basic Example" << std::endl;
    std::cout << "Version: " << ogde::VERSION_MAJOR << "." 
              << ogde::VERSION_MINOR << "." 
              << ogde::VERSION_PATCH << std::endl;

    ogde::core::Engine engine;
    
    if (engine.initialize()) {
        std::cout << "Engine initialized successfully!" << std::endl;
        
        // Main loop would go here
        // engine.run();
        
        engine.shutdown();
        std::cout << "Engine shutdown complete." << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return 1;
    }
}
