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

#include <memory>

#if GEOGL_BUILD_WITH_OPENGL == 1
#include "../../Platform/OpenGL/Rendering/OpenGLRendererAPI.hpp"
#endif

#if GEOGL_BUILD_WITH_VULKAN == 1
#include "../../Platform/Vulkan/Rendering/VulkanRendererAPI.hpp"
#endif

namespace GEOGL{


    std::string RendererAPI::getRenderingAPIName(RendererAPI::RenderingAPIEnum api) {

        switch(api){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
                return std::string("OpenGL");
            case RenderingAPIEnum::RENDERING_VULKAN_DESKTOP:
                return std::string("Vulkan");
            case RenderingAPIEnum::RENDERING_DIRECTX11_DESKTOP:
                return std::string("DirectX 11");
            case RenderingAPIEnum::RENDERING_DIRECTX12_DESKTOP:
                return std::string("DirectX 12");
            case RenderingAPIEnum::RENDERING_METAL_DESKTOP:
                return std::string("Apple Metal");
            default:
                return std::string("Unknown");
        }

    }

    std::string RendererAPI::getWindowingAPIName(RendererAPI::WindowingAPIEnum api) {

        switch(api){

            case WindowingAPIEnum::WINDOWING_GLFW_DESKTOP:
                return std::string("GLFW");
            default:
                return std::string("Unknown");

        }

    }

    bool RendererAPI::isAPISupported(RendererAPI::RenderingAPIEnum api) {
        switch (api){
            case RenderingAPIEnum::RENDERING_OPENGL_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_OPENGL && (bool) GEOGL_BUILD_WITH_GLFW;
            case RenderingAPIEnum::RENDERING_VULKAN_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_VULKAN && (bool) GEOGL_BUILD_WITH_GLFW;
            default:
                return false;
        }

    }

    Ref<RendererAPI> RendererAPI::create(RendererAPI::RenderingAPIEnum preferredAPI) {
        /* get the best API */
        RendererAPI::RenderingAPIEnum api = RenderingAPIEnum::RENDERING_INVALID;

        if(isAPISupported(preferredAPI)){
            api = preferredAPI;
        }else{

            GEOGL_CORE_ERROR_NOSTRIP("The preferred API {} is not available. Selecting one that is.", getRenderingAPIName(preferredAPI));

            /* Select most cross platform API supported */
            if(isAPISupported(RenderingAPIEnum::RENDERING_OPENGL_DESKTOP)){
                api = RenderingAPIEnum::RENDERING_OPENGL_DESKTOP;
            }else if(isAPISupported(RenderingAPIEnum::RENDERING_METAL_DESKTOP)){
                api = RenderingAPIEnum::RENDERING_METAL_DESKTOP;
            }else if(isAPISupported(RenderingAPIEnum::RENDERING_VULKAN_DESKTOP)){
                api = RenderingAPIEnum::RENDERING_VULKAN_DESKTOP;
            }else if(isAPISupported(RenderingAPIEnum::RENDERING_DIRECTX11_DESKTOP)){
                api = RenderingAPIEnum::RENDERING_DIRECTX11_DESKTOP;
            }else if(isAPISupported(RenderingAPIEnum::RENDERING_DIRECTX12_DESKTOP)){
                api = RenderingAPIEnum::RENDERING_DIRECTX12_DESKTOP;
            }

        }

        GEOGL_CORE_ASSERT_NOSTRIP(api != RenderingAPIEnum::RENDERING_INVALID, "Unable to find a suitable Rendering API.");
        GEOGL_CORE_INFO_NOSTRIP("Using {} for Rendering.", getRenderingAPIName(api));

        Ref<RendererAPI> result;
        switch(api){

            case RendererAPI::RenderingAPIEnum::RENDERING_OPENGL_DESKTOP:
#if GEOGL_BUILD_WITH_OPENGL == 1
                result = createRef<GEOGL::Platform::OpenGL::RendererAPI>();
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif


            case RendererAPI::RenderingAPIEnum::RENDERING_VULKAN_DESKTOP:
#if GEOGL_BUILD_WITH_VULKAN == 1
                result = std::make_shared<GEOGL::Platform::Vulkan::RendererAPI>();
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform Vulkan Selected but not supported!");
#endif


            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} Rendering API. Unhandled path.", RendererAPI::getRenderingAPIName(api));
                return nullptr;
        }

    }

    RendererAPI::RendererAPI(RendererAPI::RenderingAPIEnum preferredAPI) : m_API(preferredAPI){


    }

    RendererAPI::WindowingAPIEnum RendererAPI::getWindowingAPI(){

        if(WindowingAPIEnum::WINDOWING_GLFW_DESKTOP & m_API)
            return WindowingAPIEnum::WINDOWING_GLFW_DESKTOP;

        return WINDOWING_INVALID;

    }


}