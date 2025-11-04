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
    }
}

bool Engine::isRunning() const {
    return m_running;
}

} // namespace core
} // namespace ogde
