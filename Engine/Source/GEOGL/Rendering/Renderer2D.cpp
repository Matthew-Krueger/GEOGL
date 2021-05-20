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


#include "Renderer2D.hpp"
#include <GEOGL/Platform/OpenGL.hpp>
#include "RenderCommand.hpp"

namespace GEOGL{

    struct Renderer2DStorage{
        Ref<VertexArray> quadVertexArray;
        Ref<Shader> flatColorShader;
    };

    static Renderer2DStorage* s_Data;

    void GEOGL::Renderer2D::init() {

        s_Data = new Renderer2DStorage();

        s_Data->quadVertexArray = VertexArray::create();

        /* Create the quad vertex array */
        {
            std::vector<float> vertices = {-0.5f,  -0.5f,   0.0f,
                                            0.5f,  -0.5f,   0.0f,
                                            0.5f,   0.5f,   0.0f,
                                           -0.5f,   0.5f,   0.0f};

            auto vertexBuffer = VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {ShaderDataType::FLOAT3, "a_Position"},
                                        });
            }

            std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};
            auto indexBuffer = IndexBuffer::create(indices);

            s_Data->quadVertexArray->addVertexBuffer(vertexBuffer);
            s_Data->quadVertexArray->setIndexBuffer(indexBuffer);
        }

        s_Data->flatColorShader = Shader::create("SandboxResources/Shaders/FlatColor");

    }

    void GEOGL::Renderer2D::shutdown() {

        delete s_Data;

    }

    void GEOGL::Renderer2D::beginScene(const OrthographicCamera &camera) {

        s_Data->flatColorShader->bind();
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(s_Data->flatColorShader)->uploadUniformMat4("u_ProjectionViewMatrix", camera.getProjectionViewMatrix());
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(s_Data->flatColorShader)->uploadUniformMat4("u_TransformationMatrix", glm::mat4(1.0f));

    }

    void GEOGL::Renderer2D::endScene() {

    }

    void GEOGL::Renderer2D::drawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {

        drawQuad({position.x, position.y, 0}, size, color);

    }

    void GEOGL::Renderer2D::drawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color) {

        s_Data->flatColorShader->bind();
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(s_Data->flatColorShader)->uploadUniformFloat4("u_Color", color);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);

    }
}

