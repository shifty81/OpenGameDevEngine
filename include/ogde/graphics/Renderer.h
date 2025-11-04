/**
 * @file Renderer.h
 * @brief Graphics rendering system
 */

#ifndef OGDE_GRAPHICS_RENDERER_H
#define OGDE_GRAPHICS_RENDERER_H

namespace ogde {
namespace graphics {

/**
 * @class Renderer
 * @brief Main rendering system for graphics
 */
class Renderer {
public:
    /**
     * @brief Initialize the renderer
     * @return true if initialization was successful
     */
    bool initialize();

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
     * @brief Clear the screen
     */
    void clear();
};

} // namespace graphics
} // namespace ogde

#endif // OGDE_GRAPHICS_RENDERER_H
