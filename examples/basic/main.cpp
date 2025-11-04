/**
 * Basic Example
 * Demonstrates basic engine initialization and main loop
 */

#include <OpenGameDevEngine.h>
#include <iostream>

int main() {
    std::cout << "OpenGameDevEngine Basic Example" << std::endl;
    std::cout << "Version: " << ogde::VERSION_MAJOR << "." 
              << ogde::VERSION_MINOR << "." 
              << ogde::VERSION_PATCH << std::endl;
    std::cout << "Platform: " << ogde::platform::Platform::getPlatformName() << std::endl;

    ogde::core::Engine engine;
    
    // Configure engine
    ogde::core::EngineConfig config;
    config.windowTitle = "OpenGameDevEngine - Basic Example";
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.targetFPS = 60;
    
    if (!engine.initialize(config)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return 1;
    }

    // Set update callback
    uint32_t frameCount = 0;
    engine.setUpdateCallback([&](float deltaTime) {
        frameCount++;
        if (frameCount % 60 == 0) {  // Every 60 frames
            ogde::core::Logger::info("FPS: " + std::to_string(engine.getFPS()) + 
                                   ", Delta: " + std::to_string(deltaTime * 1000.0f) + "ms");
        }
    });

    // Set render callback
    engine.setRenderCallback([]() {
        // Rendering would go here
    });

    ogde::core::Logger::info("Starting engine main loop...");
    ogde::core::Logger::info("Note: On Linux, window is not created. Use Windows for full demo.");
    
#ifndef _WIN32
    // On non-Windows platforms, run for a limited time for demonstration
    ogde::core::Logger::info("Running for 3 seconds (non-Windows platform)...");
    double startTime = ogde::platform::Platform::getTime();
    engine.setUpdateCallback([&](float deltaTime) {
        frameCount++;
        if (frameCount % 60 == 0) {
            ogde::core::Logger::info("FPS: " + std::to_string(engine.getFPS()) + 
                                   ", Delta: " + std::to_string(deltaTime * 1000.0f) + "ms");
        }
        
        // Exit after 3 seconds
        if (ogde::platform::Platform::getTime() - startTime > 3.0) {
            ogde::core::Logger::info("Demo complete!");
            exit(0);
        }
    });
#endif

    // Run the engine
    engine.run();
    
    engine.shutdown();
    std::cout << "Engine shutdown complete." << std::endl;
    return 0;
}
