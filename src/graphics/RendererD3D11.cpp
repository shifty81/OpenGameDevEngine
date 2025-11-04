/**
 * DirectX 11 Renderer Implementation
 */

#ifdef _WIN32

#include "ogde/graphics/RendererD3D11.h"
#include "ogde/core/Logger.h"
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace ogde {
namespace graphics {

RendererD3D11::RendererD3D11()
    : m_initialized(false)
    , m_vsyncEnabled(true)
    , m_width(0)
    , m_height(0)
{
}

RendererD3D11::~RendererD3D11() {
    shutdown();
}

bool RendererD3D11::initialize(HWND hwnd, uint32_t width, uint32_t height, bool vsync) {
    if (m_initialized) {
        core::Logger::warning("RendererD3D11 already initialized");
        return true;
    }

    core::Logger::info("Initializing DirectX 11 renderer...");
    
    m_width = width;
    m_height = height;
    m_vsyncEnabled = vsync;

    // Create DXGI factory
    Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)dxgiFactory.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to create DXGI factory");
        return false;
    }

    // Get adapter (GPU)
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    hr = dxgiFactory->EnumAdapters(0, adapter.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to enumerate adapters");
        return false;
    }

    // Get adapter description
    DXGI_ADAPTER_DESC adapterDesc;
    adapter->GetDesc(&adapterDesc);
    
    // Convert wide string to regular string for logging
    char adapterName[128];
    size_t converted = 0;
    wcstombs_s(&converted, adapterName, sizeof(adapterName), adapterDesc.Description, _TRUNCATE);
    core::Logger::info("Using GPU: " + std::string(adapterName));

    // Swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    // Feature levels to try
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);

    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Create device, device context, and swap chain
    hr = D3D11CreateDeviceAndSwapChain(
        adapter.Get(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        createDeviceFlags,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(),
        &featureLevel,
        m_deviceContext.GetAddressOf()
    );

    if (FAILED(hr)) {
        core::Logger::error("Failed to create D3D11 device and swap chain");
        return false;
    }

    // Log feature level
    const char* featureLevelStr = "Unknown";
    switch (featureLevel) {
        case D3D_FEATURE_LEVEL_11_1: featureLevelStr = "11.1"; break;
        case D3D_FEATURE_LEVEL_11_0: featureLevelStr = "11.0"; break;
        case D3D_FEATURE_LEVEL_10_1: featureLevelStr = "10.1"; break;
        case D3D_FEATURE_LEVEL_10_0: featureLevelStr = "10.0"; break;
    }
    core::Logger::info("DirectX Feature Level: " + std::string(featureLevelStr));

    // Create render target view
    if (!createRenderTarget()) {
        core::Logger::error("Failed to create render target");
        return false;
    }

    // Create depth stencil buffer and view
    if (!createDepthStencil(width, height)) {
        core::Logger::error("Failed to create depth stencil");
        return false;
    }

    // Set render targets
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Setup viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);

    m_initialized = true;
    core::Logger::info("DirectX 11 renderer initialized successfully!");
    return true;
}

void RendererD3D11::shutdown() {
    if (!m_initialized) {
        return;
    }

    core::Logger::info("Shutting down DirectX 11 renderer...");

    // Release resources
    releaseRenderTarget();
    m_depthStencilView.Reset();
    m_depthStencilBuffer.Reset();
    
    if (m_deviceContext) {
        m_deviceContext->ClearState();
        m_deviceContext->Flush();
    }

    m_swapChain.Reset();
    m_deviceContext.Reset();
    m_device.Reset();

    m_initialized = false;
    core::Logger::info("DirectX 11 renderer shutdown complete");
}

void RendererD3D11::beginFrame() {
    if (!m_initialized) {
        return;
    }

    // Clear will be called separately
}

void RendererD3D11::endFrame() {
    if (!m_initialized || !m_swapChain) {
        return;
    }

    // Present the back buffer to the screen
    HRESULT hr = m_swapChain->Present(m_vsyncEnabled ? 1 : 0, 0);
    
    if (FAILED(hr)) {
        core::Logger::error("Failed to present swap chain");
    }
}

void RendererD3D11::clear(float r, float g, float b, float a) {
    if (!m_initialized || !m_deviceContext || !m_renderTargetView) {
        return;
    }

    // Clear render target
    float clearColor[4] = { r, g, b, a };
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

    // Clear depth stencil
    if (m_depthStencilView) {
        m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), 
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}

void RendererD3D11::resize(uint32_t width, uint32_t height) {
    if (!m_initialized || width == 0 || height == 0) {
        return;
    }

    m_width = width;
    m_height = height;

    // Release existing render target
    releaseRenderTarget();
    m_depthStencilView.Reset();
    m_depthStencilBuffer.Reset();

    // Resize swap chain buffers
    HRESULT hr = m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    if (FAILED(hr)) {
        core::Logger::error("Failed to resize swap chain buffers");
        return;
    }

    // Recreate render target
    if (!createRenderTarget()) {
        core::Logger::error("Failed to recreate render target after resize");
        return;
    }

    // Recreate depth stencil buffer and view
    if (!createDepthStencil(width, height)) {
        core::Logger::error("Failed to recreate depth stencil after resize");
        return;
    }

    // Set render targets
    m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Update viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &viewport);

    core::Logger::info("Renderer resized to " + std::to_string(width) + "x" + std::to_string(height));
}

bool RendererD3D11::createRenderTarget() {
    if (!m_swapChain) {
        return false;
    }

    // Get back buffer
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to get swap chain back buffer");
        return false;
    }

    // Create render target view
    hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to create render target view");
        return false;
    }

    return true;
}

void RendererD3D11::releaseRenderTarget() {
    m_renderTargetView.Reset();
}

bool RendererD3D11::createDepthStencil(uint32_t width, uint32_t height) {
    if (!m_device) {
        return false;
    }

    // Create depth stencil buffer
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = width;
    depthStencilDesc.Height = height;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    HRESULT hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, m_depthStencilBuffer.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to create depth stencil buffer");
        return false;
    }

    // Create depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthStencilDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    hr = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsvDesc, m_depthStencilView.GetAddressOf());
    if (FAILED(hr)) {
        core::Logger::error("Failed to create depth stencil view");
        return false;
    }

    return true;
}

bool RendererD3D11::createVertexBuffer(const void* vertices, uint32_t vertexSize, 
                                       uint32_t vertexCount, ID3D11Buffer** outBuffer) {
    if (!m_initialized || !m_device || !vertices || !outBuffer) {
        core::Logger::error("Cannot create vertex buffer: invalid parameters");
        return false;
    }

    // Create vertex buffer description
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = vertexSize * vertexCount;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    // Create subresource data
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    // Create the buffer
    HRESULT hr = m_device->CreateBuffer(&bufferDesc, &initData, outBuffer);
    if (FAILED(hr)) {
        core::Logger::error("Failed to create vertex buffer");
        return false;
    }

    core::Logger::info("Vertex buffer created successfully: " + 
                      std::to_string(vertexCount) + " vertices, " +
                      std::to_string(vertexSize) + " bytes per vertex");
    return true;
}

void RendererD3D11::setVertexBuffer(ID3D11Buffer* buffer, uint32_t vertexSize, uint32_t offset) {
    if (!m_initialized || !m_deviceContext || !buffer) {
        return;
    }

    UINT stride = vertexSize;
    UINT offsetValue = offset;
    m_deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offsetValue);
}

void RendererD3D11::draw(uint32_t vertexCount, uint32_t startVertex) {
    if (!m_initialized || !m_deviceContext) {
        return;
    }

    // Set primitive topology to triangle list
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // Draw
    m_deviceContext->Draw(vertexCount, startVertex);
}

} // namespace graphics
} // namespace ogde

#endif // _WIN32
