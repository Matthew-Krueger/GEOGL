//
// Created by matth on 2/15/21.
//

#include "OpenGLKeyCodes.hpp"

namespace GEOGL{


    int OpenGLKeyCodes::getKeyCodeImpl(KeyCode key) {
        return static_cast<int>(key);
    }

    int OpenGLKeyCodes::getMouseCodeImpl(MouseCode button) {
        return static_cast<int>(button);
    }
}