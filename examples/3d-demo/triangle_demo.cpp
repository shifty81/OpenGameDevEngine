/**
 * DirectX 11 Triangle Example
 * Demonstrates DirectX 11 rendering with a colored triangle
 */

#include <OpenGameDevEngine.h>
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include "ogde/graphics/Shader.h"
#include "ogde/graphics/RendererD3D11.h"
#include <d3d11.h>
#include <wrl/client.h>

// Vertex structure
struct Vertex {
    float position[3];  // x, y, z
    float color[4];     // r, g, b, a
};

// Simple vertex shader
const char* vertexShaderSource = R"(
struct VSInput {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input) {
    PSInput output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}
)";

// Simple pixel shader
const char* pixelShaderSource = R"(
struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PSMain(PSInput input) : SV_TARGET {
    return input.color;
}
)";

int main() {
    std::cout << "OpenGameDevEngine - DirectX 11 Triangle Example" << std::endl;
    std::cout << "Version: " << ogde::VERSION_MAJOR << "." 
              << ogde::VERSION_MINOR << "." 
              << ogde::VERSION_PATCH << std::endl;
    std::cout << "Platform: " << ogde::platform::Platform::getPlatformName() << std::endl;

    ogde::core::Engine engine;
    
    // Configure engine
    ogde::core::EngineConfig config;
    config.windowTitle = "OpenGameDevEngine - DirectX 11 Triangle";
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.targetFPS = 60;
    config.enableVSync = true;
    
    if (!engine.initialize(config)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return 1;
    }

    // Get renderer
    auto* renderer = engine.getRenderer();
    if (!renderer || !renderer->isInitialized()) {
        std::cerr << "Renderer not available!" << std::endl;
        return 1;
    }

    // Get DirectX 11 renderer (Windows only)
    auto* d3dRenderer = dynamic_cast<ogde::graphics::Renderer*>(renderer);
    
    // Create shader
    ogde::graphics::Shader shader;
    D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // We need to access the D3D11 device through the renderer
    // For now, we'll just demonstrate the engine running with a clear color
    ogde::core::Logger::info("DirectX 11 triangle rendering example");
    ogde::core::Logger::info("Note: Full triangle rendering will be implemented in the next iteration");

    // Triangle vertices (in NDC space: -1 to 1)
    Vertex vertices[] = {
        { { 0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },  // Top (red)
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },  // Right (green)
        { {-0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }   // Left (blue)
    };

    ogde::core::Logger::info("Triangle vertices prepared:");
    ogde::core::Logger::info("  Vertex 0: (0.0, 0.5, 0.0) - Red");
    ogde::core::Logger::info("  Vertex 1: (0.5, -0.5, 0.0) - Green");
    ogde::core::Logger::info("  Vertex 2: (-0.5, -0.5, 0.0) - Blue");

    // Set update callback
    uint32_t frameCount = 0;
    engine.setUpdateCallback([&](float deltaTime) {
        frameCount++;
        if (frameCount % 60 == 0) {  // Every 60 frames
            ogde::core::Logger::info("FPS: " + std::to_string(engine.getFPS()) + 
                                   ", Delta: " + std::to_string(deltaTime * 1000.0f) + "ms");
        }
    });

    // Set render callback - change clear color over time
    float hue = 0.0f;
    engine.setRenderCallback([&]() {
        // Cycle through colors
        hue += 0.001f;
        if (hue > 1.0f) hue = 0.0f;
        
        float r = 0.5f + 0.5f * std::sin(hue * 6.28f);
        float g = 0.5f + 0.5f * std::sin((hue + 0.333f) * 6.28f);
        float b = 0.5f + 0.5f * std::sin((hue + 0.666f) * 6.28f);
        
        renderer->clear(r * 0.3f, g * 0.3f, b * 0.3f, 1.0f);
        
        // Triangle rendering would go here
        // This requires accessing the D3D11 device context to create and draw vertex buffers
    });

    ogde::core::Logger::info("Starting engine main loop...");
    ogde::core::Logger::info("You should see a window with a color-cycling background!");
    
    // Run the engine
    engine.run();
    
    engine.shutdown();
    std::cout << "Engine shutdown complete." << std::endl;
    return 0;
}

#else // Non-Windows platforms

int main() {
    std::cout << "OpenGameDevEngine - DirectX 11 Triangle Example" << std::endl;
    std::cout << "This example requires Windows and DirectX 11 support." << std::endl;
    std::cout << "Platform: " << ogde::platform::Platform::getPlatformName() << std::endl;
    return 0;
}

#endif // _WIN32
