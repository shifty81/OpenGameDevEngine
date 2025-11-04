/**
 * Graphics Renderer Implementation
 */

#include "ogde/graphics/Renderer.h"
#include "ogde/core/Logger.h"

#ifdef _WIN32
#include "ogde/graphics/RendererD3D11.h"
#endif

namespace ogde {
namespace graphics {

Renderer::Renderer() {
#ifdef _WIN32
    m_rendererD3D11 = std::make_unique<RendererD3D11>();
#endif
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(void* windowHandle, uint32_t width, uint32_t height, bool vsync) {
#ifdef _WIN32
    if (m_rendererD3D11) {
        HWND hwnd = static_cast<HWND>(windowHandle);
        return m_rendererD3D11->initialize(hwnd, width, height, vsync);
    }
#endif
    
    core::Logger::warning("Renderer::initialize not implemented for this platform");
    return false;
}

void Renderer::shutdown() {
#ifdef _WIN32
    if (m_rendererD3D11) {
        m_rendererD3D11->shutdown();
    }
#endif
}

void Renderer::beginFrame() {
#ifdef _WIN32
    if (m_rendererD3D11) {
        m_rendererD3D11->beginFrame();
    }
#endif
}

void Renderer::endFrame() {
#ifdef _WIN32
    if (m_rendererD3D11) {
        m_rendererD3D11->endFrame();
    }
#endif
}

void Renderer::clear(float r, float g, float b, float a) {
#ifdef _WIN32
    if (m_rendererD3D11) {
        m_rendererD3D11->clear(r, g, b, a);
    }
#endif
}

void Renderer::resize(uint32_t width, uint32_t height) {
#ifdef _WIN32
    if (m_rendererD3D11) {
        m_rendererD3D11->resize(width, height);
    }
#endif
}

bool Renderer::isInitialized() const {
#ifdef _WIN32
    if (m_rendererD3D11) {
        return m_rendererD3D11->isInitialized();
    }
#endif
    return false;
}

#ifdef _WIN32
RendererD3D11* Renderer::getD3D11Renderer() const {
    return m_rendererD3D11.get();
}
#endif

} // namespace graphics
} // namespace ogde
