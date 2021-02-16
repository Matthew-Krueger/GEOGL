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


#include "APIManagment.hpp"
namespace GEOGL {
    enum WindowAPIType determineLowestAPI() {

        if (GEOGL_BUILD_WITH_OPENGL) {
            return WindowAPIType::WINDOW_OPENGL_DESKTOP;
        } else if (GEOGL_BUILD_WITH_VULKAN) {
            return WindowAPIType::WINDOW_VULKAN_DESKTOP;
        }

    }

    bool isAPISupported(enum WindowAPIType api) {
        switch (api){
            case WINDOW_OPENGL_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_OPENGL;
            case WINDOW_VULKAN_DESKTOP:
                return (bool) GEOGL_BUILD_WITH_VULKAN;
        }

        return false;

    }

    enum WindowAPIType findBestPreferredAPI(enum WindowAPIType preferredAPI) {

        if(isAPISupported(preferredAPI))
            return preferredAPI;

        return determineLowestAPI();

    }
}
#pragma clang diagnostic pop