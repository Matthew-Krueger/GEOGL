#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-bool-literals"
#pragma ide diagnostic ignored "bugprone-branch-clone"
#pragma ide diagnostic ignored "Simplify"

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


#include "APIManagement.hpp"
namespace GEOGL {

    std::string apiPrettyPrint(enum RenderingAPIType windowAPI){

        switch(windowAPI){
            case RenderingAPIType::API_OPENGL_DESKTOP:
                return std::string("OpenGL");
            case RenderingAPIType::API_VULKAN_DESKTOP:
                return std::string("Vulkan");
            case RenderingAPIType::API_DIRECTX11_DESKTOP:
                return std::string("DirectX 11");
            case RenderingAPIType::API_DIRECTX12_DESKTOP:
                return std::string("DirectX 12");
            case RenderingAPIType::API_METAL_DESKTOP:
                return std::string("Apple Metal");
            default:
                return std::string("Unknown");
        }

    }

    std::string windowingPrettyPrint(enum WindowingType windowing){

        switch(windowing){

            case WindowingType::WINDOWING_GLFW_DESKTOP:
                return std::string("GLFW");
            default:
                return std::string("Unknown");

        }

    }


    enum RenderingAPIType determineLowestAPI() {

        if (GEOGL_BUILD_WITH_OPENGL) {
            return RenderingAPIType::API_OPENGL_DESKTOP;
        } else if (GEOGL_BUILD_WITH_VULKAN) {
            return RenderingAPIType::API_VULKAN_DESKTOP;
        }

        GEOGL_CORE_ASSERT_NOSTRIP(false, "NO API Support detected.");
        exit(-1);

    }

    bool isAPISupported(enum RenderingAPIType api) {
        switch (api){
            case API_OPENGL_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_OPENGL && (bool) GEOGL_BUILD_WITH_GLFW;
            case API_VULKAN_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_VULKAN && (bool) GEOGL_BUILD_WITH_GLFW;
            default:
                return false;
        }

    }

    enum RenderingAPIType findBestPreferredAPI(enum RenderingAPIType preferredAPI) {

        if(isAPISupported(preferredAPI))
            return preferredAPI;

        GEOGL_CORE_ERROR_NOSTRIP("The selected api: {} is not available. Finding a new one.", apiPrettyPrint(preferredAPI));
        enum RenderingAPIType api = determineLowestAPI();
        GEOGL_CORE_ERROR_NOSTRIP("The new api will be {}.", apiPrettyPrint(api));
        return api;

    }

    APIManager::APIManager(enum RenderingAPIType api){

        m_RenderAPI = findBestPreferredAPI(api);

    }

    APIManager::~APIManager(){}

    enum WindowingType APIManager::getWindowingType() {

        if(WINDOWING_GLFW_DESKTOP & m_RenderAPI)
            return WINDOWING_GLFW_DESKTOP;

        return WINDOWING_INVALID;

    }

    enum RenderingAPIType APIManager::getRenderAPIType() {

        return m_RenderAPI;

    }
}
#pragma clang diagnostic pop