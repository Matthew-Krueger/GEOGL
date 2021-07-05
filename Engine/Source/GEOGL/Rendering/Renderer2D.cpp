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

    struct QuadVertex{
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 textureCoord;
        float tilingFactor;
        float textureIndex;
        //TODO: color, texid
    };

    struct Renderer2DData{
        const uint32_t maxQuads = 7500;
        const uint32_t maxVertices = maxQuads * 4;
        const uint32_t maxIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 32; //TODO: Get from RenderCaps

        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;

        Ref<Shader> textureShader;
        Ref<Texture2D> whiteTexture;

        uint32_t quadIndexCount = 0;
        QuadVertex* quadVertexBufferBase = nullptr;
        QuadVertex* quadVertexBufferPtr = nullptr;

        glm::vec4 quadVertexPositions[4] = {};

        //TODO: GUID identifier
        std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 0;

        Renderer2D::Statistics stats{};

        bool wireframe = false;
    };


    static Renderer2DData s_Data;
    static glm::mat4 s_IdentMatrix;

    void GEOGL::Renderer2D::init() {
        GEOGL_PROFILE_FUNCTION();

        /* Store a blank identity matrix to save on compute */
        s_IdentMatrix = glm::mat4(1.0f);

        s_Data.quadVertexArray = VertexArray::create();

        /* Create the quad vertex Buffer */
        {
            s_Data.quadVertexBuffer = VertexBuffer::create(s_Data.maxVertices * sizeof(QuadVertex));
            {
                s_Data.quadVertexBuffer->setLayout({
                                                           {ShaderDataType::FLOAT3, "a_Position"},
                                                           {ShaderDataType::FLOAT4, "a_Color"},
                                                           {ShaderDataType::FLOAT2, "a_TextureCoord"},
                                                           {ShaderDataType::FLOAT, "a_TilingFactor"},
                                                           {ShaderDataType::FLOAT, "a_TextureIndex"}
                                                   });
            }
            s_Data.quadVertexArray->addVertexBuffer(s_Data.quadVertexBuffer);
            s_Data.quadVertexBufferBase = new QuadVertex[s_Data.maxVertices];
        }

        /* Create t he quad indices */
        {


            auto *quadIndices = new uint32_t[s_Data.maxIndices];

            uint32_t offset = 0;
            for (uint32_t i = 0; i < s_Data.maxIndices; i += 6) {
                quadIndices[i + 0] = offset + 0;
                quadIndices[i + 1] = offset + 1;
                quadIndices[i + 2] = offset + 2;

                quadIndices[i + 3] = offset + 2;
                quadIndices[i + 4] = offset + 3;
                quadIndices[i + 5] = offset + 0;

                offset += 4;
            }

            auto quadIB = IndexBuffer::create(quadIndices, s_Data.maxIndices);
            s_Data.quadVertexArray->setIndexBuffer(quadIB);
            delete[] quadIndices;
        }

        /* Creat eh the white texture */
        s_Data.whiteTexture = Texture2D::create(1,1);
        uint32_t whiteTextureData = 0xFFFFFFFF;
        auto whttxtr = s_Data.whiteTexture;
        s_Data.whiteTexture->setData(&whiteTextureData, sizeof(whiteTextureData));

        /* Create the shaders */
        {
            s_Data.textureShader = Shader::create("Resources/Shaders/2DRenderer/Texture");
            s_Data.textureShader->bind();

            int32_t samplers[GEOGL::Renderer2DData::maxTextureSlots];
            for(uint32_t i = 0; i<GEOGL::Renderer2DData::maxTextureSlots; ++i){
                samplers[i] = (int32_t)i;
                s_Data.textureShader->setInt(GEOGL_FORMAT("u_Textures[{}]", i).c_str(), i);
            }
            s_Data.textureShader->setIntArray("u_Textures", samplers, GEOGL::Renderer2DData::maxTextureSlots);
        }

        s_Data.quadVertexPositions[0] = {-0.5,  -0.5,   0.0f,   1.0f};
        s_Data.quadVertexPositions[1] = { 0.5,  -0.5,   0.0f,   1.0f};
        s_Data.quadVertexPositions[2] = { 0.5,   0.5,   0.0f,   1.0f};
        s_Data.quadVertexPositions[3] = {-0.5,   0.5,   0.0f,   1.0f};

    }

    void GEOGL::Renderer2D::shutdown() {
        GEOGL_PROFILE_FUNCTION();
        if(s_Data.quadVertexBufferBase)
            delete s_Data.quadVertexBufferBase;
        s_Data.quadVertexBufferBase = nullptr;
        s_Data.quadVertexBufferPtr = nullptr;

        s_Data.quadVertexArray = nullptr;
        s_Data.quadVertexBuffer = nullptr;

        for(uint32_t i=0; i<GEOGL::Renderer2DData::maxTextureSlots; ++i){
            s_Data.textureSlots[i] = nullptr;
        }

        s_Data.whiteTexture = nullptr;

        s_Data.textureShader = nullptr;
    }

    void GEOGL::Renderer2D::beginScene(const OrthographicCamera &camera) {
        GEOGL_PROFILE_FUNCTION();

        auto projectionViewMatrix = camera.getProjectionViewMatrix();

        s_Data.textureShader->bind();
        s_Data.textureShader->setMat4("u_ProjectionViewMatrix", projectionViewMatrix);

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

        s_Data.textureSlotIndex = 0;

    }

    void GEOGL::Renderer2D::endScene() {
        GEOGL_PROFILE_FUNCTION();

        Renderer2D::flush();

        if(s_Data.wireframe){
            Renderer2D::renderWireframe(false);
        }

    }

    void GEOGL::Renderer2D::flush(){
        GEOGL_PROFILE_FUNCTION();

        if(s_Data.quadVertexBufferPtr == s_Data.quadVertexBufferBase) return; /* Since there is no data to render, skip the remainder of the function */

        s_Data.textureShader->bind();
        s_Data.quadVertexArray->bind();

        //s_Data.textureSlots[0]->bind(0);
        //s_Data.textureSlots[1]->bind(1);

        for(uint32_t i=0; i<s_Data.textureSlotIndex; ++i){
            auto& texture = s_Data.textureSlots[i];
            texture->bind(i);
        }

        uint32_t dataSize = (uint8_t*)s_Data.quadVertexBufferPtr - (uint8_t*)s_Data.quadVertexBufferBase;
        s_Data.quadVertexBuffer->setData(s_Data.quadVertexBufferBase, dataSize);

        RenderCommand::drawIndexed(s_Data.quadVertexArray, s_Data.quadIndexCount);
        s_Data.stats.drawCalls++;

        s_Data.quadIndexCount = 0;
        s_Data.quadVertexBufferPtr = s_Data.quadVertexBufferBase;

        s_Data.textureSlotIndex = 0;

    }

    void Renderer2D::renderWireframe(bool status) {

        Renderer2D::flush();
        RenderCommand::renderWireframe(&status);
        s_Data.wireframe = status;

    }

    void GEOGL::Renderer2D::drawQuad(const QuadProperties& properties) {
        GEOGL_PROFILE_FUNCTION();

        drawQuad(properties, s_Data.whiteTexture);

    }

    void Renderer2D::drawQuad(const QuadProperties& properties, const Ref<Texture2D>& texture) {
        GEOGL_PROFILE_FUNCTION();

        /* guard against buffer overflow */
        if(s_Data.quadIndexCount >= s_Data.maxIndices || s_Data.textureSlotIndex >= GEOGL::Renderer2DData::maxTextureSlots){
            flush();
        }

        glm::mat4 transform = glm::translate(s_IdentMatrix, properties.position);
        transform = glm::scale(transform, {properties.size.x, properties.size.y, 1.0f});

        glm::vec3 position = properties.position;

        float textureIndex = -1;

        for(uint32_t i=0; i<s_Data.textureSlotIndex;++i){
            if(*s_Data.textureSlots[i] == *texture){
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex==-1){
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            ++s_Data.textureSlotIndex;
        }

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;

        /* other code for not using matrix math */
        /*
        glm::mat4 transform = glm::translate(s_IdentMatrix, properties.position);
        transform = glm::scale(transform, {properties.size.x, properties.size.y, 1.0f});

        glm::vec3 position = properties.position;

        float textureIndex = -1;

        for(uint32_t i=0; i<s_Data.textureSlotIndex;++i){
            if(*s_Data.textureSlots[i] == *texture){
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex==-1){
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            ++s_Data.textureSlotIndex;
        }


        s_Data.quadVertexBufferPtr->position = {position.x-(0.5*properties.size.x), position.y-(0.5*properties.size.y), position.z};
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = {position.x+(0.5*properties.size.x), position.y-(0.5*properties.size.y), position.z};
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = {position.x+(0.5*properties.size.x), position.y+(0.5*properties.size.y), position.z};
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = {position.x-(0.5*properties.size.x), position.y+(0.5*properties.size.y), position.z};
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;
         */

        ++s_Data.stats.quadCount;

    }

    void Renderer2D::drawRotatedQuad(const Renderer2D::QuadProperties &properties, float rotation) {

        drawRotatedQuad(properties, s_Data.whiteTexture, rotation);

    }

    void Renderer2D::drawRotatedQuad(const Renderer2D::QuadProperties &properties, const Ref <Texture2D> &texture, float rotation) {

        /* guard against buffer overflow */
        if(s_Data.quadIndexCount >= s_Data.maxIndices || s_Data.textureSlotIndex >= GEOGL::Renderer2DData::maxTextureSlots){
            flush();
        }

        glm::mat4 transform = glm::translate(s_IdentMatrix, properties.position);
        transform = (rotation!=0) ? glm::rotate(transform, (rotation), {0,0,1}) : transform;
        transform = glm::scale(transform, {properties.size.x, properties.size.y, 1.0f});

        glm::vec3 position = properties.position;

        float textureIndex = -1;

        for(uint32_t i=0; i<s_Data.textureSlotIndex;++i){
            if(*s_Data.textureSlots[i] == *texture){
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex==-1){
            textureIndex = (float)s_Data.textureSlotIndex;
            s_Data.textureSlots[s_Data.textureSlotIndex] = texture;
            ++s_Data.textureSlotIndex;
        }

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[0];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[1];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,0};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[2];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {1,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadVertexBufferPtr->position = transform * s_Data.quadVertexPositions[3];
        s_Data.quadVertexBufferPtr->color = properties.colorTint;
        s_Data.quadVertexBufferPtr->textureCoord = {0,1};
        s_Data.quadVertexBufferPtr->tilingFactor = properties.tilingFactor;
        s_Data.quadVertexBufferPtr->textureIndex = textureIndex;
        s_Data.quadVertexBufferPtr++;

        s_Data.quadIndexCount += 6;

        ++s_Data.stats.quadCount;

    }

    void Renderer2D::resetStats() {

        memset(&s_Data.stats, 0, sizeof(Statistics));

    }

    Renderer2D::Statistics Renderer2D::getStatistics() {

        return s_Data.stats;

    }

}

