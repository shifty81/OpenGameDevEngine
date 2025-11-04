/**
 * @file Engine.h
 * @brief Core engine class that manages all subsystems
 */

#ifndef OGDE_CORE_ENGINE_H
#define OGDE_CORE_ENGINE_H

namespace ogde {
namespace core {

/**
 * @class Engine
 * @brief Main engine class that initializes and manages all subsystems
 */
class Engine {
public:
    /**
     * @brief Initialize the engine
     * @return true if initialization was successful
     */
    bool initialize();

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

private:
    bool m_running = false;
};

} // namespace core
} // namespace ogde

#endif // OGDE_CORE_ENGINE_H
