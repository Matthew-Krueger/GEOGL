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

#if (GEOGL_BUILD_WITH_GLFW == 1)
#include <GEOGL/Platform/GLFW.hpp>
#else
#error No windowing LIB found
#endif

namespace GEOGL{

    Window* Window::create(APIManager& api, const WindowProps& props){

        switch(api.getWindowingType()){
            case WINDOWING_GLFW_DESKTOP:
                if(GEOGL_BUILD_WITH_GLFW) {
                    return new GEOGL::Platform::GLFW::Window(api, props);
                }else{
                    GEOGL_CORE_ASSERT_NOSTRIP(false, "GLFW is not supported. Exiting.");
                    exit(-1);
                }
            default:
                GEOGL_CORE_ASSERT_NOSTRIP(false, "No api is selected. Exiting.");
        }

        return nullptr;

    }

}
