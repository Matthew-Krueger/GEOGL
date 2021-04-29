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

    void RendererAPI::setClearColor(const glm::vec4 &color) {
        m_ClearColor = color;
    }

    void RendererAPI::clear() {

        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void RendererAPI::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {

        glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

    }
}
