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

#if (GEOGL_BUILD_WITH_OPENGL == 1)
    #include "../Platform/OpenGL/IO/OpenGLWindow.hpp"
#endif

#if (GEOGL_BUILD_WITH_VULKAN == 1)
    #include "../Platform/Vulkan/IO/VulkanWindow.hpp"
#include "../Utils/APIManagment.hpp"

#endif


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

    Window* Window::create(enum WindowAPIType api,const WindowProps& props){

        bool vulkanSupported = GEOGL_BUILD_WITH_VULKAN;
        bool openGLSupported = GEOGL_BUILD_WITH_OPENGL;

        switch(api){
            case WINDOW_OPENGL_DESKTOP:
                GEOGL_CORE_ASSERT_NOSTRIP(openGLSupported, "OpenGL is selected as the API, but does not appear to be supported.");
#if GEOGL_BUILD_WITH_OPENGL == true
                return new GEOGL::Platform::OpenGL::OpenGLWindow(props);
#endif
                break;
            case WINDOW_VULKAN_DESKTOP:
                GEOGL_CORE_ASSERT_NOSTRIP(vulkanSupported, "Vulkan is selected as the API, but does not appear to be supported.");
#if GEOGL_BUILD_WITH_VULKAN == true
                return new GEOGL::VulkanWindow(props);
#endif
                break;
            default:
                GEOGL_CORE_ASSERT_NOSTRIP(false, "No api is selected. Exiting.");
        }

        return nullptr;

    }

}
