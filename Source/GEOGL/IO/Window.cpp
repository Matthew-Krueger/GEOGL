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


#include "Window.hpp"

//#if (GEOGL_BUILD_WITH_OPENGL == true)
#include "../Platform/OpenGL/OpenGLWindow.hpp"


namespace GEOGL{

    std::string apiPrettyPrint(enum WindowAPIType windowAPI){

        switch(windowAPI){
            case WindowAPIType::WINDOW_OPENGL_DESKTOP:
                return std::string("OpenGL");
            case WindowAPIType::WINDOW_VULKAN_DESKTOP:
                return std::string("Vulkan");
            default:
                return std::string("Unknown");
        }

    }

    Window* Window::create(const WindowProps& props){
        return new OpenGLWindow(props);
    }

}
