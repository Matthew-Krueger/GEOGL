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


#include "Renderer.hpp"
#include "../../Platform/OpenGL/Rendering/OpenGLShader.hpp"

namespace GEOGL{

    Renderer::SceneData* Renderer::m_SceneData = new SceneData;

    void Renderer::beginScene(const OrthographicCamera& camera) {

        m_SceneData->projectionViewMatrix = camera.getProjectionViewMatrix();

    }

    void Renderer::endScene() {

    }

    void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,  const glm::mat4& transform) {
        shader->bind();
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(shader)->uploadUniformMat4("u_ProjectionViewMatrix", m_SceneData->projectionViewMatrix);
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(shader)->uploadUniformMat4("u_TransformMatrix", transform);
        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }
}