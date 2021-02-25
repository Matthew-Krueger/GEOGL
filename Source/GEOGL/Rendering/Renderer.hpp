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


#ifndef GEOGL_RENDERER_HPP
#define GEOGL_RENDERER_HPP

#include "RenderCommand.hpp"

namespace GEOGL{

    class GEOGL_API Renderer{
    public:

        inline static void setClearColor(const glm::vec4& color){ RenderCommand::setClearColor(color); };

        /**
         * \brief Begins the scene, setting up the renderer
         */
        static void beginScene();

        /**
         * \brief Ends the scene (and will eventually dispatch on the rendercommandque)
         */
        static void endScene();

        /**
         * \brief Submit a VertexArray for drawing
         * @param vertexArray The vertex array to submit
         */
        static void submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static const std::shared_ptr<RendererAPI>& getRendererAPI() { return RenderCommand::getRendererAPI(); };
        inline static void setRendererAPI(std::shared_ptr<RendererAPI> rendererApi) { RenderCommand::setRendererAPI(rendererApi); };


    };

}

#endif //GEOGL_RENDERER_HPP