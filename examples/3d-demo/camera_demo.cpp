/**
 * 3D Camera Example
 * Demonstrates the camera system with a rotating 3D cube
 */

#include <OpenGameDevEngine.h>
#include <iostream>
#include <cmath>

#ifdef _WIN32
#include "ogde/graphics/Shader.h"
#include "ogde/graphics/RendererD3D11.h"
#include "ogde/graphics/Camera.h"
#include <d3d11.h>
#include <wrl/client.h>

// Vertex structure with position and color
struct Vertex {
    float position[3];  // x, y, z
    float color[4];     // r, g, b, a
};

// Constant buffer for MVP matrix
struct ConstantBuffer {
    float mvpMatrix[16];  // Model-View-Projection matrix
};

// Vertex shader with MVP transformation
const char* vertexShaderSource = R"(
cbuffer ConstantBuffer : register(b0) {
    matrix mvpMatrix;
};

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
    output.position = mul(float4(input.position, 1.0f), mvpMatrix);
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
    std::cout << "OpenGameDevEngine - 3D Camera Example" << std::endl;
    std::cout << "Version: " << ogde::VERSION_MAJOR << "." 
              << ogde::VERSION_MINOR << "." 
              << ogde::VERSION_PATCH << std::endl;

    ogde::core::Engine engine;
    
    // Configure engine
    ogde::core::EngineConfig config;
    config.windowTitle = "OpenGameDevEngine - 3D Camera Demo";
    config.windowWidth = 800;
    config.windowHeight = 600;
    config.targetFPS = 60;
    config.enableVSync = true;
    
    if (!engine.initialize(config)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return 1;
    }

    auto* renderer = engine.getRenderer();
    if (!renderer || !renderer->isInitialized()) {
        std::cerr << "Renderer not available!" << std::endl;
        return 1;
    }

    ogde::core::Logger::info("3D Camera example with rotating cube");

    // Cube vertices (8 corners)
    Vertex vertices[] = {
        // Front face
        { {-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },  // 0
        { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },  // 1
        { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },  // 2
        { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // 3
        // Back face
        { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },  // 4
        { { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },  // 5
        { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f, 1.0f} },  // 6
        { {-0.5f,  0.5f,  0.5f}, {0.5f, 0.5f, 0.5f, 1.0f} }   // 7
    };

    // Cube indices for 12 triangles (36 vertices)
    uint16_t indices[] = {
        // Front face
        0, 1, 2,  0, 2, 3,
        // Right face
        1, 5, 6,  1, 6, 2,
        // Back face
        5, 4, 7,  5, 7, 6,
        // Left face
        4, 0, 3,  4, 3, 7,
        // Top face
        3, 2, 6,  3, 6, 7,
        // Bottom face
        4, 5, 1,  4, 1, 0
    };

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

    // Create vertex buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    if (!d3d11Renderer->createVertexBuffer(vertices, sizeof(Vertex), 8, 
                                          vertexBuffer.GetAddressOf())) {
        ogde::core::Logger::error("Failed to create vertex buffer!");
        return 1;
    }

    // Create index buffer
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices;

    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, indexBuffer.GetAddressOf()))) {
        ogde::core::Logger::error("Failed to create index buffer!");
        return 1;
    }

    // Create constant buffer for MVP matrix
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(ConstantBuffer);
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    if (FAILED(device->CreateBuffer(&cbDesc, nullptr, constantBuffer.GetAddressOf()))) {
        ogde::core::Logger::error("Failed to create constant buffer!");
        return 1;
    }

    // Create camera
    ogde::graphics::Camera camera;
    camera.setPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    camera.lookAt(0.0f, 2.0f, -5.0f,  // Eye position
                  0.0f, 0.0f, 0.0f,    // Look at origin
                  0.0f, 1.0f, 0.0f);   // Up vector
    camera.update();

    ogde::core::Logger::info("Camera created and positioned");

    // Animation variables
    float rotation = 0.0f;

    // Set update callback
    engine.setUpdateCallback([&](float deltaTime) {
        // Rotate cube
        rotation += deltaTime * 45.0f;  // 45 degrees per second
        if (rotation > 360.0f) rotation -= 360.0f;
    });

    // Set render callback
    engine.setRenderCallback([&]() {
        renderer->clear(0.1f, 0.1f, 0.15f, 1.0f);
        
        // Create model matrix (rotation)
        float rotRad = rotation * 3.14159f / 180.0f;
        float cosR = std::cos(rotRad);
        float sinR = std::sin(rotRad);
        
        // Simple rotation around Y axis (column-major)
        float modelMatrix[16] = {
            cosR,  0.0f, sinR, 0.0f,
            0.0f,  1.0f, 0.0f, 0.0f,
            -sinR, 0.0f, cosR, 0.0f,
            0.0f,  0.0f, 0.0f, 1.0f
        };

        // Get view-projection matrix from camera
        const float* vpMatrix = camera.getViewProjectionMatrix();

        // Calculate MVP = VP * Model (matrix multiplication)
        ConstantBuffer cb;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                cb.mvpMatrix[i * 4 + j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    cb.mvpMatrix[i * 4 + j] += vpMatrix[i * 4 + k] * modelMatrix[k * 4 + j];
                }
            }
        }

        // Update constant buffer
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        if (SUCCEEDED(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
            memcpy(mappedResource.pData, &cb, sizeof(ConstantBuffer));
            deviceContext->Unmap(constantBuffer.Get(), 0);
        }

        // Bind shader and buffers
        shader.bind(deviceContext);
        deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        d3d11Renderer->setVertexBuffer(vertexBuffer.Get(), sizeof(Vertex));
        deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Draw the cube
        deviceContext->DrawIndexed(36, 0, 0);
    });

    ogde::core::Logger::info("Starting engine main loop...");
    ogde::core::Logger::info("You should see a rotating 3D cube!");
    
    // Run the engine
    engine.run();
    
    engine.shutdown();
    std::cout << "Engine shutdown complete." << std::endl;
    
    return 0;
}

#else // Non-Windows platforms

int main() {
    std::cout << "OpenGameDevEngine - 3D Camera Example" << std::endl;
    std::cout << "This example requires Windows and DirectX 11 support." << std::endl;
    std::cout << "Platform: " << ogde::platform::Platform::getPlatformName() << std::endl;
    return 0;
}

#endif // _WIN32
