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

    enum RenderingAPIType {
        API_INVALID = 0,
        API_OPENGL_DESKTOP = BIT(0),
        API_VULKAN_DESKTOP = BIT(1),
        API_DIRECTX11_DESKTOP = BIT(2),
        API_DIRECTX12_DESKTOP = BIT(3),
        API_METAL_DESKTOP = BIT(4),
    };

    enum WindowingType {

        WINDOWING_INVALID = 0,
        WINDOWING_GLFW_DESKTOP = BIT(0) | BIT(1),
        WINDOWING_WIN32_DESKTOP = BIT(2) | BIT(3),
        WINDOWING_METAL_DESKTOP = BIT(4)

    };

    /**
     * \brief Pretty prints the API Type.
     * @return The Pretty Printed API type
     */
    GEOGL_API std::string apiPrettyPrint(enum RenderingAPIType API);

    GEOGL_API std::string windowingPrettyPrint(enum WindowingType windowing);

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
    GEOGL_API enum RenderingAPIType determineLowestAPI();

    /**
     * \brief Determines if the selected API is supported or not.
     * @param api The API to check.
     * @return If it is supported.
     */
    GEOGL_API bool isAPISupported(enum RenderingAPIType api);

    /**
     * \brief Finds the preferred API if the current one is not supported.
     * @param preferredAPI The API that the client prefers to use.
     * @return The API the engine will use.
     */
    GEOGL_API enum RenderingAPIType findBestPreferredAPI(enum RenderingAPIType preferredAPI);

    class GEOGL_API RendererAPI{
    public:
        RendererAPI(enum RenderingAPIType api = RenderingAPIType::API_INVALID);
        ~RendererAPI();

        enum WindowingType getWindowingType() const;
        enum RenderingAPIType getRenderAPIType() const;

    private:
        enum RenderingAPIType m_RenderAPI;

    };

}

#endif //GEOGL_APIMANAGER_HPP