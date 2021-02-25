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


#include "RendererAPI.hpp"
#include "../Modules/Platform/OpenGL/Rendering/OpenGLRendererAPI.hpp"

namespace GEOGL{


    std::string RendererAPI::getRenderingAPIName(RendererAPI::RenderingAPI api) {

        switch(api){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
                return std::string("OpenGL");
            case RenderingAPI::RENDERING_VULKAN_DESKTOP:
                return std::string("Vulkan");
            case RenderingAPI::RENDERING_DIRECTX11_DESKTOP:
                return std::string("DirectX 11");
            case RenderingAPI::RENDERING_DIRECTX12_DESKTOP:
                return std::string("DirectX 12");
            case RenderingAPI::RENDERING_METAL_DESKTOP:
                return std::string("Apple Metal");
            default:
                return std::string("Unknown");
        }

    }

    std::string RendererAPI::getWindowingAPIName(RendererAPI::WindowingAPI api) {

        switch(api){

            case WindowingAPI::WINDOWING_GLFW_DESKTOP:
                return std::string("GLFW");
            default:
                return std::string("Unknown");

        }

    }

    bool RendererAPI::isAPISupported(RendererAPI::RenderingAPI api) {
        switch (api){
            case RenderingAPI::RENDERING_OPENGL_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_OPENGL && (bool) GEOGL_BUILD_WITH_GLFW;
            case RenderingAPI::RENDERING_VULKAN_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_VULKAN && (bool) GEOGL_BUILD_WITH_GLFW;
            default:
                return false;
        }

    }

    std::shared_ptr<RendererAPI> RendererAPI::create(RendererAPI::RenderingAPI preferredAPI) {
        /* get the best API */
        RendererAPI::RenderingAPI api = RenderingAPI::RENDERING_INVALID;

        if(isAPISupported(preferredAPI)){
            api = preferredAPI;
        }else{

            GEOGL_CORE_ERROR_NOSTRIP("The preferred API {} is not available. Selecting one that is.", getRenderingAPIName(preferredAPI));

            /* Select most cross platform API supported */
            if(isAPISupported(RenderingAPI::RENDERING_OPENGL_DESKTOP)){
                api = RenderingAPI::RENDERING_OPENGL_DESKTOP;
            }else if(isAPISupported(RenderingAPI::RENDERING_METAL_DESKTOP)){
                api = RenderingAPI::RENDERING_METAL_DESKTOP;
            }else if(isAPISupported(RenderingAPI::RENDERING_VULKAN_DESKTOP)){
                api = RenderingAPI::RENDERING_VULKAN_DESKTOP;
            }else if(isAPISupported(RenderingAPI::RENDERING_DIRECTX11_DESKTOP)){
                api = RenderingAPI::RENDERING_DIRECTX11_DESKTOP;
            }else if(isAPISupported(RenderingAPI::RENDERING_DIRECTX12_DESKTOP)){
                api = RenderingAPI::RENDERING_DIRECTX12_DESKTOP;
            }

        }

        GEOGL_CORE_ASSERT_NOSTRIP(api != RenderingAPI::RENDERING_INVALID, "Unable to find a suitable Rendering API.");
        GEOGL_CORE_INFO_NOSTRIP("Using {} for Rendering.", getRenderingAPIName(api));

        std::shared_ptr<RendererAPI> result;
        switch(api){
            case RendererAPI::RenderingAPI::RENDERING_OPENGL_DESKTOP:
#ifdef GEOGL_BUILD_WITH_OPENGL
                result.reset(new GEOGL::Platform::OpenGL::RendererAPI());
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", RendererAPI::getRenderingAPIName(api));
                return nullptr;
        }

    }

    RendererAPI::RendererAPI(RendererAPI::RenderingAPI preferredAPI) : m_API(preferredAPI){


    }

    RendererAPI::WindowingAPI RendererAPI::getWindowingAPI(){

        if(WindowingAPI::WINDOWING_GLFW_DESKTOP & m_API)
            return WindowingAPI::WINDOWING_GLFW_DESKTOP;

        return WINDOWING_INVALID;

    }


}