/**
 * Basic Example
 * Demonstrates basic engine initialization and main loop with DirectX rendering
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

    ogde::core::Logger::info("Basic rendering example with animated triangle");

    // Triangle vertices (in NDC space: -1 to 1)
    Vertex vertices[] = {
        { { 0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },  // Top (red)
        { {-0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },  // Bottom-left (blue)
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }   // Bottom-right (green)
    };

    // Get the platform-specific D3D11 renderer
    auto* d3d11Renderer = renderer->getD3D11Renderer();
    
    if (!d3d11Renderer || !d3d11Renderer->getDevice()) {
        ogde::core::Logger::error("Failed to get D3D11 device!");
        return 1;
    }

    ID3D11Device* device = d3d11Renderer->getDevice();
    ID3D11DeviceContext* deviceContext = d3d11Renderer->getDeviceContext();

    // Create shader
    ogde::graphics::Shader shader;
    D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    if (!shader.createFromSource(device, vertexShaderSource, pixelShaderSource, 
                                 inputLayout, ARRAYSIZE(inputLayout))) {
        ogde::core::Logger::error("Failed to create shader!");
        return 1;
    }

    ogde::core::Logger::info("Shader created successfully!");

    // Create vertex buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    if (!d3d11Renderer->createVertexBuffer(vertices, sizeof(Vertex), 3, 
                                          vertexBuffer.GetAddressOf())) {
        ogde::core::Logger::error("Failed to create vertex buffer!");
        return 1;
    }

    ogde::core::Logger::info("Vertex buffer created successfully!");

    // Set update callback
    uint32_t frameCount = 0;
    engine.setUpdateCallback([&](float deltaTime) {
        frameCount++;
        if (frameCount % 60 == 0) {  // Every 60 frames
            ogde::core::Logger::info("FPS: " + std::to_string(engine.getFPS()) + 
                                   ", Delta: " + std::to_string(deltaTime * 1000.0f) + "ms");
        }
    });

    // Set render callback with animated background
    float hue = 0.0f;
    engine.setRenderCallback([&]() {
        // Cycle through colors
        hue += 0.0005f;
        if (hue > 1.0f) hue = 0.0f;
        
        float r = 0.5f + 0.5f * std::sin(hue * 6.28f);
        float g = 0.5f + 0.5f * std::sin((hue + 0.333f) * 6.28f);
        float b = 0.5f + 0.5f * std::sin((hue + 0.666f) * 6.28f);
        
        renderer->clear(r * 0.2f, g * 0.2f, b * 0.2f, 1.0f);
        
        // Bind shader and render triangle
        shader.bind(deviceContext);
        d3d11Renderer->setVertexBuffer(vertexBuffer.Get(), sizeof(Vertex));
        d3d11Renderer->draw(3);
    });

    ogde::core::Logger::info("Starting engine main loop...");
    ogde::core::Logger::info("You should see a window with a colored triangle on a color-cycling background!");
    
    // Run the engine
    engine.run();
    
    engine.shutdown();
    std::cout << "Engine shutdown complete." << std::endl;
    return 0;
}

#else // Non-Windows platforms

int main() {
    std::cout << "OpenGameDevEngine Basic Example" << std::endl;
    std::cout << "This example requires Windows and DirectX 11 support." << std::endl;
    std::cout << "Platform: " << ogde::platform::Platform::getPlatformName() << std::endl;
    return 0;
}

#endif // _WIN32
