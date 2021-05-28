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
#include <glad/glad.h>
#include "OpenGLRendererAPI.hpp"

namespace GEOGL::Platform::OpenGL{


    /* Since this is the OpenGL Rendering api, we know the api is OpenGL */
    RendererAPI::RendererAPI() : GEOGL::RendererAPI(RENDERING_OPENGL_DESKTOP){

    }

    RendererAPI::~RendererAPI() {

    }

    void RendererAPI::init(){
        GEOGL_PROFILE_FUNCTION();

        GEOGL_CORE_INFO("Initializing the RendererAPI");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

        /* Log GPU information */
        {
            GEOGL_CORE_INFO_NOSTRIP("OpenGL Info:");
            GEOGL_CORE_INFO_NOSTRIP("   Vendor: {}", glGetString(GL_VENDOR));
            GEOGL_CORE_INFO_NOSTRIP("   Renderer: {}", glGetString(GL_RENDERER));
            GEOGL_CORE_INFO_NOSTRIP("   OpenGL Version: {}.", (const char *) glGetString(GL_VERSION));
            GEOGL_CORE_INFO_NOSTRIP("   GLSL Version {}.", (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION));
        }

    }

    void RendererAPI::setViewport(const glm::ivec2 &dimensions) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glViewport(0,0,(GLsizei) dimensions.x, (GLsizei) dimensions.y);

    }

    void RendererAPI::setClearColor(const glm::vec4 &color) {
        m_ClearColor = color;
    }

    void RendererAPI::clear() {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void RendererAPI::drawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        GLsizei count = indexCount ? (GLsizei) indexCount : (GLsizei) vertexArray->getIndexBuffer()->getCount();

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    }

    void RendererAPI::renderWireframe(bool *status) {

        if(*status){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    }

}
