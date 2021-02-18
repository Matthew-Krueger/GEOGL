//
// Created by matth on 2/15/21.
//

#include "OpenGLKeyCodes.hpp"

namespace GEOGL::Platform::OpenGL{


    int OpenGLKeyCodes::getNativeKeyCodeImpl(KeyCode key) {
        return static_cast<int>(key);
    }

    int OpenGLKeyCodes::getNativeMouseCodeImpl(MouseCode button) {
        return static_cast<int>(button);
    }

    KeyCode OpenGLKeyCodes::getGEOGLKeyCodeImpl(int nativeKeyCode) {
        return static_cast<KeyCode> (nativeKeyCode);
    }

    MouseCode OpenGLKeyCodes::getGEOGLMouseCodeImpl(int nativeMouseCode) {
        return static_cast<MouseCode> (nativeMouseCode);
    }
}