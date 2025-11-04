/**
 * Core Engine Implementation
 */

#include "ogde/core/Engine.h"

namespace ogde {
namespace core {

bool Engine::initialize() {
    m_running = true;
    return true;
}

void Engine::shutdown() {
    m_running = false;
}

void Engine::run() {
    while (m_running) {
        // Main game loop logic would go here
        // TODO: Add frame rate limiting, input processing, update, and render calls
        // For now, this is a placeholder that should be called manually in a controlled loop
        break; // Exit immediately in template implementation
    }
}

bool Engine::isRunning() const {
    return m_running;
}

} // namespace core
} // namespace ogde
