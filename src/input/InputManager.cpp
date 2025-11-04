/**
 * Input Manager Implementation
 */

#include "ogde/input/InputManager.h"

namespace ogde {
namespace input {

bool InputManager::initialize() {
    return true;
}

void InputManager::shutdown() {
}

void InputManager::update() {
}

bool InputManager::isKeyDown(KeyCode key) const {
    return false;
}

bool InputManager::isMouseButtonDown(MouseButton button) const {
    return false;
}

void InputManager::getMousePosition(int& x, int& y) const {
    x = 0;
    y = 0;
}

} // namespace input
} // namespace ogde
