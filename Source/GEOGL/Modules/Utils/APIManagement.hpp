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


#ifndef GEOGL_APIMANAGER_HPP
#define GEOGL_APIMANAGER_HPP

namespace GEOGL{

    enum WindowAPIType {
        WINDOW_OPENGL_DESKTOP = 0,
        WINDOW_VULKAN_DESKTOP = 1
    };

    /**
     * \brief Pretty prints the API Type.
     * @return The Pretty Printed API type
     */
    GEOGL_API std::string apiPrettyPrint(enum WindowAPIType windowAPI);

    /**
     * \brief Determines the lowest supported API by the binary
     *
     * This function will, using macros, determine the lowest supported API specification (by the binary)
     * at runtime, and return it when ask during runtime.
     *
     * \note The lowest supported API specification is only what the binary supports, not what is actually
     * supported by the computer.
     * @return The lowest supported API Specification
     */
    GEOGL_API enum WindowAPIType determineLowestAPI();

    /**
     * \brief Determines if the selected API is supported or not.
     * @param api The API to check.
     * @return If it is supported.
     */
    GEOGL_API bool isAPISupported(enum WindowAPIType api);

    /**
     * \brief Finds the preferred API if the current one is not supported.
     * @param preferredAPI The API that the client prefers to use.
     * @return The API the engine will use.
     */
    GEOGL_API enum WindowAPIType findBestPreferredAPI(enum WindowAPIType preferredAPI);

}

#endif //GEOGL_APIMANAGER_HPP
