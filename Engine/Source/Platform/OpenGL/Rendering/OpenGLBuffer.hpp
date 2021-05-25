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

#ifndef GEOGL_OPENGLBUFFER_HPP
#define GEOGL_OPENGLBUFFER_HPP

#include "../../../GEOGL/Rendering/Buffer.hpp"

namespace GEOGL::Platform::OpenGL{

    class GEOGL_API VertexBuffer : public GEOGL::VertexBuffer{
    public:
        VertexBuffer(uint32_t size);
        VertexBuffer(float* vertices, uint32_t size);
        virtual ~VertexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;
        virtual void setData(const void* data, uint32_t size) override;

        inline void setLayout(const BufferLayout& layout) override { m_Layout = layout; };
        inline const BufferLayout& getLayout() const override { return m_Layout; };


    private:
        uint32_t m_Size;
        uint32_t m_VBOID;
        BufferLayout m_Layout;
    };


    class GEOGL_API IndexBuffer : public GEOGL::IndexBuffer{
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~IndexBuffer();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual inline uint32_t getCount() const override {return m_Count; };

    private:
        uint32_t m_Count;
        uint32_t m_IndexBufferID;

    };

}

#endif //GEOGL_OPENGLBUFFER_HPP
