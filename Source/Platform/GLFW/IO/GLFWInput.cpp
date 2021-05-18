/*******************************************************************************
 * Copyright (c) 2020 Matthew Krueger                                          *
 *                                                                             *
 * This software is provided 'as-is', without any express or implied           *
 * warranty. In no event will the authors be held liable for any damages       *
 * arising from the use of this software.                                      *
 *                                                                             *
 * Permission is granted to anyone to use this software for any purpose,       *
 * including commercial applications, and to alter it and redistribute it      *
 * freely, subject to the following restrictions:                              *
 *                                                                             *
 * 1. The origin of this software must not be misrepresented; you must not     *
 *    claim that you wrote the original software. If you use this software     *
 *    in a product, an acknowledgment in the product documentation would       *
 *    be appreciated but is not required.                                      *
 *                                                                             *
 * 2. Altered source versions must be plainly marked as such, and must not     *
 *    be misrepresented as being the original software.                        *
 *                                                                             *
 * 3. This notice may not be removed or altered from any source                *
 *    distribution.                                                            *
 *                                                                             *
 *******************************************************************************/

/*******************************************************************************
 *                                                                             *
 * This code was based heavily off the Cherno game engine series               *
 *                                                                             *
 *******************************************************************************/

#include "GLFWInput.hpp"
#include "../../../Utils/InputCodesConverter.hpp"
#include "../../../GEOGL/Application/Application.hpp"

#include <GLFW/glfw3.h>

namespace GEOGL::Platform::GLFW{


    bool Input::isKeyPressedImpl(KeyCode keycode) {

        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, InputCodesConverter::getNativeKeyCode(keycode));

        return state == GLFW_PRESS || state == GLFW_REPEAT;

    }

    bool Input::isMouseButtonPressedImpl(MouseCode button) {

        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, InputCodesConverter::getNativeMouseCode(button));

        return state == GLFW_PRESS;

    }

    bool Input::getMouseXImpl() {

        return getMousePositionImpl().x;

    }

    bool Input::getMouseYImpl() {

        return getMousePositionImpl().y;

    }

    glm::vec2 Input::getMousePositionImpl(){

        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;

        glfwGetCursorPos(window, &xpos, &ypos);

        return glm::vec2((float)xpos, (float)ypos);

    }

}

#include "GLFWInput.hpp"
