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

#include "Camera.hpp"
#include "RenderCommand.hpp"
#include "Shader.hpp"

namespace GEOGL{

    class GEOGL_API Renderer{
    public:

        /**
         * Sets the clear color
         * @param color
         */
        inline static void setClearColor(const glm::vec4& color){ RenderCommand::setClearColor(color); };

        inline static void clear(){ RenderCommand::clear(); };

        /**
         * \brief Begins the scene, setting up the renderer
         */
        static void beginScene(const OrthographicCamera& camera);

        /**
         * \brief Ends the scene (and will eventually dispatch on the rendercommandque)
         */
        static void endScene();

        /**
         * \brief Submit a VertexArray for drawing
         * @param vertexArray The vertex array to submit
         */
        static void submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        /**
         * Gets the rendererAPI
         * @return
         */
        inline static const Ref<RendererAPI>& getRendererAPI() { return RenderCommand::getRendererAPI(); };

        /**
         * Sets the rendererAPI
         * @param rendererApi the Api Instance to set
         */
        inline static void setRendererAPI(Ref<RendererAPI> rendererApi) { RenderCommand::setRendererAPI(rendererApi); };

    private:
        struct SceneData{
            glm::mat4 projectionViewMatrix;
        };

        static SceneData* m_SceneData;

    };

}

#endif //GEOGL_RENDERER_HPP
