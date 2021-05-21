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
#include "OpenGLBuffer.hpp"


namespace GEOGL::Platform::OpenGL{


    /*
     * Vertex Buffer
     */

    VertexBuffer::VertexBuffer(const std::vector<float> &vertices) {
        GEOGL_PROFILE_FUNCTION();

        /* Obtain a copy of vertices */
        m_CPUData = vertices;

        /* Do some evil pointer manipulation to get a void* to the data, so I can access GLM's memory */
        void* vector = (void*)m_CPUData.data();
        uint32_t vectorSizeBytes = m_CPUData.size() * sizeof(float); // Times size of glm::vec3 component vector.

        /* Now, upload to the GPU */
        {
            GEOGL_PROFILE_SCOPE("Create Buffer");
            glCreateBuffers(1, &m_VBOID);
        }

        {
            GEOGL_PROFILE_SCOPE("Bind Buffer");
            glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
        }

        {
            GEOGL_PROFILE_SCOPE("Upload Buffer");
            glBufferData(GL_ARRAY_BUFFER, vectorSizeBytes, vector, GL_STATIC_DRAW);
        }


    }

    VertexBuffer::~VertexBuffer() {
        GEOGL_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_VBOID);

    }

    void VertexBuffer::bind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);

    }

    void VertexBuffer::unbind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }


    /*
     *  Index Buffer
     */
    IndexBuffer::IndexBuffer(const std::vector<uint32_t> &indices) {
        GEOGL_PROFILE_FUNCTION();

        /* Obtain a copy of vertices */
        m_CPUData = indices;
        m_CPUDataSizeCache = static_cast<uint32_t>(m_CPUData.size());

        uint64_t vectorSizeBytes = m_CPUData.size() * sizeof(uint32_t); // Times size of glm::vec3 component vector.

        {
            GEOGL_PROFILE_SCOPE("Create Buffer");
            glCreateBuffers(1, &m_IndexBufferID);
        }

        {
            GEOGL_PROFILE_SCOPE("Bind Buffer");
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
        }

        {
            GEOGL_PROFILE_SCOPE("Upload Buffer");
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectorSizeBytes, (void*)m_CPUData.data(), GL_STATIC_DRAW);
        }


    }

    IndexBuffer::~IndexBuffer() {
        GEOGL_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &m_IndexBufferID);

    }

    void IndexBuffer::bind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);

    }

    void IndexBuffer::unbind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    }
}
