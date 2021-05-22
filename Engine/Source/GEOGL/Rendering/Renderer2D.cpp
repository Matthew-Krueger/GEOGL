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
        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;
    };

    static Renderer2DStorage* s_Data;
    static glm::mat4 s_IdentMatrix;

    void GEOGL::Renderer2D::init() {
        GEOGL_PROFILE_FUNCTION();

        s_Data = new Renderer2DStorage();

        /* Store a blank identity matrix to save on compute */
        s_IdentMatrix = glm::mat4(1.0f);

        s_Data->quadVertexArray = VertexArray::create();

        /* Create the quad vertex array */
        {
            std::vector<float> vertices = {-0.5f,  -0.5f,   0.0f,   0.0f,   0.0f,
                                            0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,
                                            0.5f,   0.5f,   0.0f,   1.0f,   1.0f,
                                           -0.5f,   0.5f,   0.0f,   0.0f,   1.0f};

            auto vertexBuffer = VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {ShaderDataType::FLOAT3, "a_Position"},
                                                {ShaderDataType::FLOAT2, "a_TextureCoord"}
                                        });
            }

            std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};
            auto indexBuffer = IndexBuffer::create(indices);

            s_Data->quadVertexArray->addVertexBuffer(vertexBuffer);
            s_Data->quadVertexArray->setIndexBuffer(indexBuffer);
        }

        /* Creat eh the white texture */
        s_Data->whiteTexture = Texture2D::create(1,1);
        uint32_t whiteTextureData = 0xFFFFFFFF;
        s_Data->whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

        /* Create the shaders */
        {
            s_Data->textureShader = Shader::create("Resources/Shaders/2DRenderer/Texture");
            s_Data->textureShader->setInt("u_Texture", 0);
        }

    }

    void GEOGL::Renderer2D::shutdown() {
        GEOGL_PROFILE_FUNCTION();

        delete s_Data;

    }

    void GEOGL::Renderer2D::beginScene(const OrthographicCamera &camera) {
        GEOGL_PROFILE_FUNCTION();

        auto projectionViewMatrix = camera.getProjectionViewMatrix();

        s_Data->textureShader->bind();
        s_Data->textureShader->setMat4("u_ProjectionViewMatrix", projectionViewMatrix);

    }

    void GEOGL::Renderer2D::endScene() {
        GEOGL_PROFILE_FUNCTION();

    }

    void GEOGL::Renderer2D::drawQuad(const QuadProperties& properties) {
        GEOGL_PROFILE_FUNCTION();

        drawQuad(properties, s_Data->whiteTexture);

    }

    void Renderer2D::drawQuad(const QuadProperties& properties, const Ref<Texture2D>& texture) {
        GEOGL_PROFILE_FUNCTION();

        s_Data->textureShader->bind();
        texture->bind(0);
        s_Data->textureShader->setFloat4("u_TintColor", properties.colorTint);
        s_Data->textureShader->setFloat("u_TilingFactor", properties.tilingFactor);

        /* Build transform and upload */
        glm::mat4 transform = glm::translate(s_IdentMatrix, properties.position);
        transform = (properties.rotation!=0) ? glm::rotate(transform, (properties.rotation), {0,0,1}) : transform;
        transform = glm::scale(transform, {properties.size.x, properties.size.y, 1.0f});
        s_Data->textureShader->setMat4("u_TransformationMatrix", transform);

        s_Data->quadVertexArray->bind();
        RenderCommand::drawIndexed(s_Data->quadVertexArray);

    }
}

