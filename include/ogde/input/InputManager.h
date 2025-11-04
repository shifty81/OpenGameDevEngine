/**
 * @file InputManager.h
 * @brief Input handling system
 */

#ifndef OGDE_INPUT_INPUTMANAGER_H
#define OGDE_INPUT_INPUTMANAGER_H

namespace ogde {
namespace input {

/**
 * @enum KeyCode
 * @brief Keyboard key codes
 */
enum class KeyCode {
    Unknown,
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Space, Enter, Escape
};

/**
 * @enum MouseButton
 * @brief Mouse button codes
 */
enum class MouseButton {
    Left,
    Right,
    Middle
};

/**
 * @class InputManager
 * @brief Handles keyboard, mouse, and controller input
 */
class InputManager {
public:
    /**
     * @brief Initialize the input manager
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Shutdown the input manager
     */
    void shutdown();

    /**
     * @brief Update input state
     */
    void update();

    /**
     * @brief Check if a key is currently pressed
     * @param key The key to check
     * @return true if the key is pressed
     */
    bool isKeyDown(KeyCode key) const;

    /**
     * @brief Check if a mouse button is pressed
     * @param button The button to check
     * @return true if the button is pressed
     */
    bool isMouseButtonDown(MouseButton button) const;

    /**
     * @brief Get mouse position
     * @param x Output X coordinate
     * @param y Output Y coordinate
     */
    void getMousePosition(int& x, int& y) const;
};

} // namespace input
} // namespace ogde

#endif // OGDE_INPUT_INPUTMANAGER_H
