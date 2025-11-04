/**
 * @file Engine.h
 * @brief Core engine class that manages all subsystems
 */

#ifndef OGDE_CORE_ENGINE_H
#define OGDE_CORE_ENGINE_H

#include <memory>
#include <functional>

namespace ogde {
namespace platform {
    class WindowWin32;
}

namespace graphics {
    class Renderer;
}

namespace core {

/**
 * @brief Engine configuration structure
 */
struct EngineConfig {
    const char* windowTitle = "OpenGameDevEngine";
    uint32_t windowWidth = 1280;
    uint32_t windowHeight = 720;
    bool enableVSync = true;
    uint32_t targetFPS = 60;
};

/**
 * @class Engine
 * @brief Main engine class that initializes and manages all subsystems
 */
class Engine {
public:
    /**
     * @brief Constructor
     */
    Engine();

    /**
     * @brief Destructor
     */
    ~Engine();

    /**
     * @brief Initialize the engine
     * @param config Engine configuration
     * @return true if initialization was successful
     */
    bool initialize(const EngineConfig& config = EngineConfig());

    /**
     * @brief Shutdown the engine and cleanup resources
     */
    void shutdown();

    /**
     * @brief Run the main engine loop
     */
    void run();

    /**
     * @brief Check if the engine is running
     * @return true if the engine is running
     */
    bool isRunning() const;

    /**
     * @brief Set update callback
     * @param callback Function to call each frame for updates
     */
    void setUpdateCallback(std::function<void(float)> callback);

    /**
     * @brief Set render callback
     * @param callback Function to call each frame for rendering
     */
    void setRenderCallback(std::function<void()> callback);

    /**
     * @brief Get delta time (time since last frame in seconds)
     * @return Delta time in seconds
     */
    float getDeltaTime() const { return m_deltaTime; }

    /**
     * @brief Get frames per second
     * @return Current FPS
     */
    float getFPS() const { return m_fps; }

    /**
     * @brief Get the renderer
     * @return Pointer to the renderer
     */
    graphics::Renderer* getRenderer() const { return m_renderer.get(); }

private:
    void updateTiming();
    void updateFPS();

    bool m_running;
    EngineConfig m_config;
    
    // Timing
    double m_lastFrameTime;
    float m_deltaTime;
    float m_fps;
    uint32_t m_frameCount;
    double m_fpsUpdateTime;

#ifdef _WIN32
    // Platform
    std::unique_ptr<platform::WindowWin32> m_window;
#endif

    // Graphics
    std::unique_ptr<graphics::Renderer> m_renderer;

    // Callbacks
    std::function<void(float)> m_updateCallback;
    std::function<void()> m_renderCallback;
};

} // namespace core
} // namespace ogde

#endif // OGDE_CORE_ENGINE_H
