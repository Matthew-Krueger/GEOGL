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
#include "OpenGLVertexArray.hpp"
#include "OpenGLShader.hpp"

namespace GEOGL::Platform::OpenGL{


    VertexArray::VertexArray(){
        GEOGL_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
        glBindVertexArray(0);

    }

    VertexArray::~VertexArray(){
        GEOGL_PROFILE_FUNCTION();
        //GEOGL_CORE_CRITICAL_NOSTRIP("Destroyed the vao #{}", m_RendererID);

        /* Bind the vertex array so we no longer have the VBOs bound */
        glBindVertexArray(m_RendererID);
        for(const auto& vertexBuffer : m_VertexBuffers){
            vertexBuffer->unbind();
        }
        m_IndexBuffer->unbind();
        setBoundID(0);

        /* now, delete the VAO */
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &m_RendererID);

    }

    void VertexArray::bind() const{
        GEOGL_RENDERER_PROFILE_FUNCTION();

        if(m_RendererID != getBoundID()) {
            //GEOGL_CORE_CRITICAL_NOSTRIP("Binding the VAO #{}", m_RendererID);
            glBindVertexArray(m_RendererID);
            setBoundID(m_RendererID);
        }

    }

    void VertexArray::unbind() const{
        GEOGL_RENDERER_PROFILE_FUNCTION();

        setBoundID(0);
        glBindVertexArray(0);

    }

    void VertexArray::addVertexBuffer(const Ref<GEOGL::VertexBuffer> &vertexBuffer){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        GEOGL_CORE_ASSERT_NOSTRIP(!vertexBuffer->getLayout().getElements().empty(),
                                  "OpenGL Vertex Buffer elements appears to be empty. Please call setLayout() with a valid BufferLayout on your VertexBuffer before calling {}().", __func__);

        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        uint32_t index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for(const BufferElement& element : layout){

            /* Hopefully this doesn't crash */
            void* elementOffsetPtr = reinterpret_cast<void*>(element.offset);

            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                    index,
                    element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.dataType),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    layout.getStride(),
                    elementOffsetPtr);

            index++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
        glBindVertexArray(0);

    }

    void VertexArray::setIndexBuffer(const Ref<GEOGL::IndexBuffer> &indexBuffer){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->bind();

        m_IndexBuffer = indexBuffer;
        glBindVertexArray(0);

    }

}