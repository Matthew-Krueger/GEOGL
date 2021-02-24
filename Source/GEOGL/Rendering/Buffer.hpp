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

#ifndef GEOGL_BUFFER_HPP
#define GEOGL_BUFFER_HPP

#include "Shader.hpp"

namespace GEOGL{

    struct GEOGL_API BufferElement{

        std::string name;
        ShaderDataType dataType;
        uint32_t offset;
        uint32_t size;
        bool normalized;

        BufferElement() = default;
        inline BufferElement(ShaderDataType dataType, const std::string& name, bool normalized = false)
            : name(name), dataType(dataType) , size(shaderDataTypeSize(dataType)), offset(0), normalized(normalized){};

        uint32_t getComponentCount() const;
    };

    class GEOGL_API BufferLayout{
    public:
        BufferLayout() = default;
        inline BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {

            calculateOffsetAndStride();

        };

        inline uint32_t getStride() const { return m_Stride; };
        inline const std::vector<BufferElement>& getElements() const {return m_Elements;};

        inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); };
        inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); };
        inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); };
        inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); };
    private:
        void calculateOffsetAndStride();
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride;

    };

    class GEOGL_API VertexBuffer{
    public:
        virtual ~VertexBuffer(){};

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void setLayout(const BufferLayout& layout) = 0;
        virtual const BufferLayout& getLayout() const = 0;

        static std::shared_ptr<VertexBuffer> create(const std::vector<float>& vertices);
    };


    class GEOGL_API IndexBuffer{
    public:
        virtual ~IndexBuffer(){};

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;

        static std::shared_ptr<IndexBuffer> create(const std::vector<uint32_t>& indices);

    };

}

#endif //GEOGL_BUFFER_HPP
