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

    /**
     * \brief Stores the information for a BufferElement to be used in a BufferLayout
     */
    struct GEOGL_API BufferElement{

        /**
         * \brief The name of the variable in the shader
         */
        std::string name;

        /**
         * \brief The ShaderDataType representing the type of data (e.g. vec3, mat4x4, int, etc.)
         */
        ShaderDataType dataType;

        /**
         * \brief The offset of this particular element from the beginning of the layout
         */
        uint32_t offset;

        /**
         * \brief The size (in bytes) of this element
         */
        uint32_t size;

        /**
         * \brief Whether or not the data is normalized. Defaults to false.
         */
        bool normalized;

        /**
         * Defines a default constructor
         */
        BufferElement() = default;

        /**
         * \brief Constructs a BufferElement using a dataType and name. normalized is by default false.
         * @param dataType The ShaderDataType of the data.
         * @param name The name of the variable in the shader
         * @param normalized Whether or not the data is normalized.
         */
        inline BufferElement(ShaderDataType dataType, const std::string& name, bool normalized = false)
            : name(name), dataType(dataType) , size(shaderDataTypeSize(dataType)), offset(0), normalized(normalized){};

        uint32_t getComponentCount() const;
    };

    /**
     * \brief Defines a BufferLayout to be used in a VertexBuffer to define how the components are packed.
     */
    class GEOGL_API BufferLayout{
    public:

        /**
         * \brief Defines a default constructor
         */
        BufferLayout() = default;

        /**
         * \brief Constructs a BufferLayout using an initializer list of elements.
         *
         * Usage:
         * VertexBuffer->setLayout({
         *
         *  {ShaderDataType::FLOAT3, "position"},
         *  {ShaderDataType::FLOAT3, "normal"},
         *  {ShaderDataType::FLOAT2, "textureCoords"}
         *
         * })
         *
         * @param elements An initializer_list of elements to use to initialize the vector with.
         */
        inline BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {

            calculateOffsetAndStride();

        };

        /**
         * \brief Gets the stride of the data. I.E. how big in size all of the elements put together are.
         * @return The Stride of the data.
         */
        inline uint32_t getStride() const { return m_Stride; };

        /**
         * \brief Gets the vector of elements used in the VertexBuffer
         * @return The vector of BufferElement s
         */
        inline const std::vector<BufferElement>& getElements() const {return m_Elements;};

        /**
         * \brief Gets an iterator for the beginning of the BufferElement vector.
         * @return std::iterator of the vector
         */
        inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); };

        /**
         * \brief Gets an iterator for the end of the BufferElement vector.
         * @return std::iterator of the vector
         */
        inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); };

        /**
         * \brief Gets an iterator for the beginning of the BufferElement vector.
         * @return std::const_iterator of the vector
         */
        inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); };

        /**
         * \brief Gets an iterator for the end of the BufferElement vector.
         * @return std::iterator of the vector
         */
        inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); };
    private:

        /**
         * \brief Iterates through the vector and sets the stride and offset of each of the buffer locations
         */
        void calculateOffsetAndStride();

        /**
         * \brief Stores the elements of the buffer
         */
        std::vector<BufferElement> m_Elements;

        /**
         * \brief Stores the stride of all of the elements
         */
        uint32_t m_Stride;

    };

    /**
     * \brief Describes a VertexBuffer interface.
     */
    class GEOGL_API VertexBuffer{
    public:
        virtual ~VertexBuffer(){};

        /**
         * \brief Binds a vertex buffer for drawing
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds a vertex buffer
         *
         * \note A vertex buffer should never be unbound as long as it is contained in a VAO with OpenGL
         */
        virtual void unbind() const = 0;

        /**
         * \brief Sets the layout of the VertexBuffer. Use an initializer list.
         *
         * Usage:
         * VertexBuffer->setLayout({
         *
         *  {ShaderDataType::FLOAT3, "position"},
         *  {ShaderDataType::FLOAT3, "normal"},
         *  {ShaderDataType::FLOAT2, "textureCoords"}
         *
         * })
         *
         * @param layout The layout to set. Can be an initializer list.
         */
        virtual void setLayout(const BufferLayout& layout) = 0;

        /**
         * \brief Gets the layout stored in the buffer that describes the data contained in the buffer.
         * @return The BufferLayout stored that describes the data in the buffer.
         */
        virtual const BufferLayout& getLayout() const = 0;

        /**
         * \brief Creates a VertexBuffer using the API stored in the Application singleton.
         * @param vertices The data to upload to the GPU
         * @return The VertexBuffer object using the selected API.
         */
        static std::shared_ptr<VertexBuffer> create(const std::vector<float>& data);
    };


    /**
     * \brief Describes an index buffer interface.
     */
    class GEOGL_API IndexBuffer{
    public:
        virtual ~IndexBuffer(){};

        /**
         * \brief Binds an IndexBuffer for drawing.
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds an IndexBuffer
         *
         * \note In OpenGL, an IndexBuffer should never be unbound outside of the VAO.
         */
        virtual void unbind() const = 0;

        /**
         * \brief Gets the number of vertices in the model
         *
         * \note For speed, this value should be cached by implementations of the buffer interface.
         *
         * @return The number of vertices in the model
         */
        virtual uint32_t getCount() const = 0;

        /**
         * \brief Creates a IndexBuffer using the API stored in the Application singleton.
         * @param indices The data to upload to the GPU
         * @return The IndexBuffer object using the selected API.
         */
        static std::shared_ptr<IndexBuffer> create(const std::vector<uint32_t>& indices);

    };

}

#endif //GEOGL_BUFFER_HPP
