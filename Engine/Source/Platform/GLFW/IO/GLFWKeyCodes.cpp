//
// Created by matth on 2/15/21.
//

#include "GLFWKeyCodes.hpp"

namespace GEOGL::Platform::GLFW{


    int GLFWKeyCodes::getNativeKeyCodeImpl(KeyCode key) {
        return static_cast<int>(key);
    }

    int GLFWKeyCodes::getNativeMouseCodeImpl(MouseCode button) {
        return static_cast<int>(button);
    }

    KeyCode GLFWKeyCodes::getGEOGLKeyCodeImpl(int nativeKeyCode) {
        return static_cast<KeyCode> (nativeKeyCode);
    }

    MouseCode GLFWKeyCodes::getGEOGLMouseCodeImpl(int nativeMouseCode) {
        return static_cast<MouseCode> (nativeMouseCode);
    }
}