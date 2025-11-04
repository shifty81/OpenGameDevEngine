/**
 * Core Engine Implementation
 */

#include "ogde/core/Engine.h"
#include "ogde/core/Logger.h"
#include "ogde/platform/Platform.h"

#ifdef _WIN32
#include "ogde/platform/WindowWin32.h"
#endif

namespace ogde {
namespace core {

Engine::Engine()
    : m_running(false)
    , m_lastFrameTime(0.0)
    , m_deltaTime(0.0f)
    , m_fps(0.0f)
    , m_frameCount(0)
    , m_fpsUpdateTime(0.0)
{
}

Engine::~Engine() {
    shutdown();
}

bool Engine::initialize(const EngineConfig& config) {
    m_config = config;

    Logger::info("Initializing OpenGameDevEngine...");
    Logger::info("Platform: " + platform::Platform::getPlatformName());

    // Initialize platform
    if (!platform::Platform::initialize()) {
        Logger::error("Failed to initialize platform!");
        return false;
    }

#ifdef _WIN32
    // Create window
    m_window = std::make_unique<platform::WindowWin32>();
    if (!m_window->create(m_config.windowTitle, m_config.windowWidth, m_config.windowHeight)) {
        Logger::error("Failed to create window!");
        return false;
    }

    Logger::info("Window created: " + std::string(m_config.windowTitle));

    // Set window callbacks
    m_window->setCloseCallback([this]() {
        Logger::info("Window close requested");
        m_running = false;
    });

    m_window->setResizeCallback([this](uint32_t width, uint32_t height) {
        Logger::info("Window resized: " + std::to_string(width) + "x" + std::to_string(height));
    });
#endif

    m_running = true;
    m_lastFrameTime = platform::Platform::getTime();
    m_fpsUpdateTime = m_lastFrameTime;

    Logger::info("Engine initialized successfully!");
    return true;
}

void Engine::shutdown() {
    if (!m_running) {
        return;
    }

    Logger::info("Shutting down engine...");

#ifdef _WIN32
    // Destroy window
    if (m_window) {
        m_window->destroy();
        m_window.reset();
    }
#endif

    // Shutdown platform
    platform::Platform::shutdown();

    m_running = false;
    Logger::info("Engine shutdown complete");
}

void Engine::run() {
    Logger::info("Starting main loop...");

    while (m_running) {
#ifdef _WIN32
        // Process window messages
        if (m_window && !m_window->processMessages()) {
            m_running = false;
            break;
        }
#endif

        // Update timing
        updateTiming();

        // Update
        if (m_updateCallback) {
            m_updateCallback(m_deltaTime);
        }

        // Render
        if (m_renderCallback) {
            m_renderCallback();
        }

        // Update FPS counter
        updateFPS();

        // Frame rate limiting (if not using VSync)
        if (!m_config.enableVSync && m_config.targetFPS > 0) {
            double targetFrameTime = 1.0 / m_config.targetFPS;
            double frameEndTime = platform::Platform::getTime();
            double currentFrameTime = frameEndTime - m_lastFrameTime;
            
            if (currentFrameTime < targetFrameTime) {
                uint32_t sleepTime = static_cast<uint32_t>((targetFrameTime - currentFrameTime) * 1000.0);
                platform::Platform::sleep(sleepTime);
            }
        }
    }

    Logger::info("Main loop ended");
}

bool Engine::isRunning() const {
    return m_running;
}

void Engine::setUpdateCallback(std::function<void(float)> callback) {
    m_updateCallback = callback;
}

void Engine::setRenderCallback(std::function<void()> callback) {
    m_renderCallback = callback;
}

void Engine::updateTiming() {
    double currentTime = platform::Platform::getTime();
    m_deltaTime = static_cast<float>(currentTime - m_lastFrameTime);
    
    // Clamp delta time to prevent issues when debugging or window loses focus
    // Maximum delta time of 0.1 seconds (10 FPS minimum)
    const float maxDeltaTime = 0.1f;
    if (m_deltaTime > maxDeltaTime) {
        m_deltaTime = maxDeltaTime;
    }
    
    m_lastFrameTime = currentTime;
}

void Engine::updateFPS() {
    m_frameCount++;
    double currentTime = platform::Platform::getTime();
    double elapsed = currentTime - m_fpsUpdateTime;

    if (elapsed >= 1.0) {
        m_fps = static_cast<float>(m_frameCount) / static_cast<float>(elapsed);
        m_frameCount = 0;
        m_fpsUpdateTime = currentTime;
    }
}

} // namespace core
} // namespace ogde
