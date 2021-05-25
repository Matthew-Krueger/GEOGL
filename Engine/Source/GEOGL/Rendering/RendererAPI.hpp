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


#ifndef GEOGL_RENDERERAPI_HPP
#define GEOGL_RENDERERAPI_HPP

#include "VertexArray.hpp"

namespace GEOGL{

    /**
     * \brief Describes a RendererAPI interface.
     */
    class GEOGL_API RendererAPI{
    public:
        /**
         * \brief Represents the rendering API to be used
         *
         * Uses bit shifting to be able to deduce the correct windowing api. for instance, the binary of API_OPENGL_DESKTOP
         * is 0001 while API
         */
        enum GEOGL_API RenderingAPIEnum {
            RENDERING_INVALID = 0,
            RENDERING_OPENGL_DESKTOP = BIT(0),
            RENDERING_VULKAN_DESKTOP = BIT(1),
            RENDERING_DIRECTX11_DESKTOP = BIT(2),
            RENDERING_DIRECTX12_DESKTOP = BIT(3),
            RENDERING_METAL_DESKTOP = BIT(4),
        };

        /**
         * \brief Represents the api to be used for windowing (i.e. win32, glfw, etc)
         */
        enum GEOGL_API WindowingAPIEnum {

            WINDOWING_INVALID = 0,
            WINDOWING_GLFW_DESKTOP = BIT(0) | BIT(1),
            WINDOWING_WIN32_DESKTOP = BIT(2) | BIT(3),
            WINDOWING_METAL_DESKTOP = BIT(4)

        };


        /**
         * \brief Gets the name of the rendering API
         * @param api The api of which to get the name
         * @return The human readable name.
         */
        static std::string getRenderingAPIName(RenderingAPIEnum api);

        /**
         * \brief Gets the name of the Windowing API
         * @param api The api of which to get the name
         * @return The human readable name.
         */
        static std::string getWindowingAPIName(WindowingAPIEnum api);
        static bool isAPISupported(RenderingAPIEnum api);

        static Ref<RendererAPI> create(RendererAPI::RenderingAPIEnum preferredAPI);

    public:

        virtual void init() = 0;
        virtual void setViewport(const glm::ivec2& dimensions) = 0;

        virtual ~RendererAPI() = default;

        /**
         * \brief Sets the color to clear the screen before drawing each frame.
         * @param color
         */
        virtual void setClearColor(const glm::vec4& color) = 0;

        /**
         * \brief Clears the screen
         */
        virtual void clear() = 0;

        /**
         * Draws an indexed VertexArray.
         * @param vertexArray The VertexArray to draw
         */
        virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

        inline RenderingAPIEnum getRenderingAPI() { return m_API; } ;
        RendererAPI::WindowingAPIEnum getWindowingAPI();
        inline uint32_t getNumberDrawCalls() { return m_DrawCalls; };
        inline void resetDrawCalls() { m_DrawCalls = 0; };

    protected:
        /**
         * \brief Constructs a RendererAPI with the specified RenderingAPIEnum. The implementation is free to ignore this however.
         * @param preferredAPI The preferred api to use for rendering
         */
        RendererAPI(RendererAPI::RenderingAPIEnum preferredAPI);
        uint32_t m_DrawCalls;

    private:
        RenderingAPIEnum m_API;



    };

}

#endif //GEOGL_RENDERERAPI_HPP
