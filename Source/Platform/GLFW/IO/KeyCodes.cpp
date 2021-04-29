//
// Created by matth on 2/15/21.
//

#include "KeyCodes.hpp"

namespace GEOGL::Platform::GLFW{


    int KeyCodes::getNativeKeyCodeImpl(KeyCode key) {
        return static_cast<int>(key);
    }

    int KeyCodes::getNativeMouseCodeImpl(MouseCode button) {
        return static_cast<int>(button);
    }

    KeyCode KeyCodes::getGEOGLKeyCodeImpl(int nativeKeyCode) {
        return static_cast<KeyCode> (nativeKeyCode);
    }

    MouseCode KeyCodes::getGEOGLMouseCodeImpl(int nativeMouseCode) {
        return static_cast<MouseCode> (nativeMouseCode);
    }
}