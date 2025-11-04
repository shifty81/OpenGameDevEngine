/**
 * @file RendererD3D11.h
 * @brief DirectX 11 rendering implementation
 */

#ifndef OGDE_GRAPHICS_RENDERERD3D11_H
#define OGDE_GRAPHICS_RENDERERD3D11_H

#ifdef _WIN32

#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <memory>
#include <vector>

// Forward declarations
struct HWND__;
typedef HWND__* HWND;

namespace ogde {
namespace graphics {

/**
 * @class RendererD3D11
 * @brief DirectX 11 rendering implementation
 */
class RendererD3D11 {
public:
    RendererD3D11();
    ~RendererD3D11();

    /**
     * @brief Initialize the DirectX 11 renderer
     * @param hwnd Window handle
     * @param width Window width
     * @param height Window height
     * @param vsync Enable vertical synchronization
     * @return true if initialization was successful
     */
    bool initialize(HWND hwnd, uint32_t width, uint32_t height, bool vsync = true);

    /**
     * @brief Shutdown the renderer and release resources
     */
    void shutdown();

    /**
     * @brief Begin a new frame
     */
    void beginFrame();

    /**
     * @brief End the current frame and present to screen
     */
    void endFrame();

    /**
     * @brief Clear the render target with a color
     * @param r Red component (0.0 - 1.0)
     * @param g Green component (0.0 - 1.0)
     * @param b Blue component (0.0 - 1.0)
     * @param a Alpha component (0.0 - 1.0)
     */
    void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

    /**
     * @brief Resize the renderer's buffers
     * @param width New width
     * @param height New height
     */
    void resize(uint32_t width, uint32_t height);

    /**
     * @brief Get the Direct3D device
     * @return Pointer to ID3D11Device
     */
    ID3D11Device* getDevice() const { return m_device.Get(); }

    /**
     * @brief Get the Direct3D device context
     * @return Pointer to ID3D11DeviceContext
     */
    ID3D11DeviceContext* getDeviceContext() const { return m_deviceContext.Get(); }

    /**
     * @brief Check if the renderer is initialized
     * @return true if initialized
     */
    bool isInitialized() const { return m_initialized; }

    /**
     * @brief Create a vertex buffer
     * @param vertices Pointer to vertex data
     * @param vertexSize Size of a single vertex in bytes
     * @param vertexCount Number of vertices
     * @param outBuffer Output pointer to store the created vertex buffer
     * @return true if successful
     */
    bool createVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, 
                           ID3D11Buffer** outBuffer);

    /**
     * @brief Draw vertices using the current vertex buffer
     * @param vertexCount Number of vertices to draw
     * @param startVertex Starting vertex index
     */
    void draw(uint32_t vertexCount, uint32_t startVertex = 0);

    /**
     * @brief Set the active vertex buffer
     * @param buffer Vertex buffer to set
     * @param vertexSize Size of a single vertex in bytes
     * @param offset Offset in bytes from the start of the buffer
     */
    void setVertexBuffer(ID3D11Buffer* buffer, uint32_t vertexSize, uint32_t offset = 0);

private:
    /**
     * @brief Create the render target view
     * @return true if successful
     */
    bool createRenderTarget();

    /**
     * @brief Release the render target view
     */
    void releaseRenderTarget();

    /**
     * @brief Create depth stencil buffer and view
     * @param width Buffer width
     * @param height Buffer height
     * @return true if successful
     */
    bool createDepthStencil(uint32_t width, uint32_t height);

    bool m_initialized;
    bool m_vsyncEnabled;
    uint32_t m_width;
    uint32_t m_height;

    // DirectX 11 objects
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState;
};

} // namespace graphics
} // namespace ogde

#endif // _WIN32

#endif // OGDE_GRAPHICS_RENDERERD3D11_H
