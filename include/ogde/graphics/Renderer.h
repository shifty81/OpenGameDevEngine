/**
 * @file Renderer.h
 * @brief Graphics rendering system
 */

#ifndef OGDE_GRAPHICS_RENDERER_H
#define OGDE_GRAPHICS_RENDERER_H

#include <memory>
#include <cstdint>

#ifdef _WIN32
// Forward declaration for Windows handle
struct HWND__;
typedef HWND__* HWND;
#endif

namespace ogde {
namespace graphics {

#ifdef _WIN32
class RendererD3D11;
#endif

/**
 * @class Renderer
 * @brief Main rendering system for graphics (platform abstraction)
 */
class Renderer {
public:
    Renderer();
    ~Renderer();

    /**
     * @brief Initialize the renderer
     * @param windowHandle Platform-specific window handle (HWND on Windows)
     * @param width Window width
     * @param height Window height
     * @param vsync Enable vertical synchronization
     * @return true if initialization was successful
     */
    bool initialize(void* windowHandle, uint32_t width, uint32_t height, bool vsync = true);

    /**
     * @brief Shutdown the renderer
     */
    void shutdown();

    /**
     * @brief Begin a new frame
     */
    void beginFrame();

    /**
     * @brief End the current frame and present
     */
    void endFrame();

    /**
     * @brief Clear the screen with a color
     * @param r Red component (0.0 - 1.0)
     * @param g Green component (0.0 - 1.0)
     * @param b Blue component (0.0 - 1.0)
     * @param a Alpha component (0.0 - 1.0)
     */
    void clear(float r = 0.0f, float g = 0.2f, float b = 0.4f, float a = 1.0f);

    /**
     * @brief Resize the renderer's buffers
     * @param width New width
     * @param height New height
     */
    void resize(uint32_t width, uint32_t height);

    /**
     * @brief Check if the renderer is initialized
     * @return true if initialized
     */
    bool isInitialized() const;

private:
#ifdef _WIN32
    std::unique_ptr<RendererD3D11> m_rendererD3D11;
#endif
};

} // namespace graphics
} // namespace ogde

#endif // OGDE_GRAPHICS_RENDERER_H
