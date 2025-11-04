/**
 * @file Window.h
 * @brief Window management system
 */

#ifndef OGDE_GRAPHICS_WINDOW_H
#define OGDE_GRAPHICS_WINDOW_H

#include <string>

namespace ogde {
namespace graphics {

/**
 * @class Window
 * @brief Window creation and management
 */
class Window {
public:
    /**
     * @brief Create a window
     * @param title Window title
     * @param width Window width
     * @param height Window height
     * @return true if window was created successfully
     */
    bool create(const std::string& title, int width, int height);

    /**
     * @brief Close the window
     */
    void close();

    /**
     * @brief Check if the window should close
     * @return true if the window should close
     */
    bool shouldClose() const;

    /**
     * @brief Update the window
     */
    void update();

private:
    bool m_shouldClose = false;
};

} // namespace graphics
} // namespace ogde

#endif // OGDE_GRAPHICS_WINDOW_H
