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

#ifndef GEOGL_RENDERCOMMAND_HPP
#define GEOGL_RENDERCOMMAND_HPP

#include "VertexArray.hpp"
#include "RendererAPI.hpp"

namespace GEOGL{

    class GEOGL_API RenderCommand{
    public:

        /**
         * Executes a set clear color command
         * @param color
         */
        inline static void setClearColor(const glm::vec4& color){ s_RendererAPI->setClearColor(color); };

        /**
         * Executes a clear command
         */
        inline static void clear(){ s_RendererAPI->clear(); };

        /**
         * Draws an index vertexArray
         * @param vertexArray
         */
        inline static void drawIndexed(const Ref<VertexArray>& vertexArray){ s_RendererAPI->drawIndexed(vertexArray); };

        /**
         * Gets the rendererAPI
         * @return
         */
        inline static const Ref<RendererAPI>& getRendererAPI() { return s_RendererAPI; };

        /**
         * Sets the RendererAPI.
         * @param rendererApi
         */
        inline static void setRendererAPI(Ref<RendererAPI>& rendererApi) { s_RendererAPI = rendererApi; };

    private:
        static Ref<RendererAPI> s_RendererAPI;
    };

}

#endif //GEOGL_RENDERCOMMAND_HPP
