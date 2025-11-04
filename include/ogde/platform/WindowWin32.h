/**
 * @file WindowWin32.h
 * @brief Windows-specific window implementation
 */

#ifndef OGDE_PLATFORM_WINDOWWIN32_H
#define OGDE_PLATFORM_WINDOWWIN32_H

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <string>
#include <functional>

namespace ogde {
namespace platform {

/**
 * @brief Windows-specific window class
 */
class WindowWin32 {
public:
    /**
     * @brief Window event callback types
     */
    using ResizeCallback = std::function<void(uint32_t width, uint32_t height)>;
    using CloseCallback = std::function<void()>;

    /**
     * @brief Constructor
     */
    WindowWin32();

    /**
     * @brief Destructor
     */
    ~WindowWin32();

    /**
     * @brief Create and show the window
     * @param title Window title
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @return true if window was created successfully
     */
    bool create(const std::string& title, uint32_t width, uint32_t height);

    /**
     * @brief Destroy the window
     */
    void destroy();

    /**
     * @brief Process window messages (call in main loop)
     * @return true if window should continue running
     */
    bool processMessages();

    /**
     * @brief Get the window handle (HWND)
     * @return Window handle
     */
    HWND getHandle() const { return m_hwnd; }

    /**
     * @brief Get window width
     * @return Width in pixels
     */
    uint32_t getWidth() const { return m_width; }

    /**
     * @brief Get window height
     * @return Height in pixels
     */
    uint32_t getHeight() const { return m_height; }

    /**
     * @brief Check if window is active
     * @return true if window is active
     */
    bool isActive() const { return m_isActive; }

    /**
     * @brief Set resize callback
     * @param callback Callback function
     */
    void setResizeCallback(ResizeCallback callback) { m_resizeCallback = callback; }

    /**
     * @brief Set close callback
     * @param callback Callback function
     */
    void setCloseCallback(CloseCallback callback) { m_closeCallback = callback; }

private:
    /**
     * @brief Window procedure (static)
     */
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Handle window messages
     */
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    HINSTANCE m_hinstance;
    std::string m_title;
    uint32_t m_width;
    uint32_t m_height;
    bool m_isActive;
    bool m_shouldClose;

    ResizeCallback m_resizeCallback;
    CloseCallback m_closeCallback;
};

} // namespace platform
} // namespace ogde

#endif // _WIN32

#endif // OGDE_PLATFORM_WINDOWWIN32_H
