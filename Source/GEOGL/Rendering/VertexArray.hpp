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


#ifndef GEOGL_VERTEXARRAY_HPP
#define GEOGL_VERTEXARRAY_HPP

#include "Buffer.hpp"

namespace GEOGL{

    /**
     * \brief Represents a Vertex Array interface.
     *
     * This vertex array interface handles all of the code for binding and unbinding all of the vertex array
     * information.
     */
    class GEOGL_API VertexArray{
    public:
        virtual ~VertexArray(){};

        /**
         * \brief Binds a vertex array for rendering
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds a vertex array
         */
        virtual void unbind() const = 0;

        /**
         * \brief Adds a vertex buffer to the vertex array.
         *
         * Adds and binds a vertex buffer to this vertex array so that it can render with this vertex array.
         *
         * \note This class keeps a shared_ptr reference to the vertex buffer, so if it is no longer needed,
         * you may allow the vertex buffer to go out of scope.
         *
         * \note The VertexBuffer MUST have set a BufferLayout, obtainable with doing VertexBuffer->setLayout().
         * See \link GEOGL::VertexBuffer::setLayout \endlink
         *
         * @param vertexBuffer A VertexBuffer to bind to this VAO. A reference is kept with shared_ptr
         */
        virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;

        /**
         * \brief Sets the index buffer for this vertex array.
         *
         * Sets and binds an index buffer to this vertex array so that it can render with this vertex array.
         *
         * \note This class keeps a shared_ptr reference to the index buffer, so if it is no longer needed,
         * you may allow the vertex buffer to go out of scope.
         *
         * @param indexBuffer The index buffer that is to be set.
         */
        virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        /**
         * \brief Gets the VertexBuffers that are used with this VertexArray.
         * @return The VertexBuffers associated with VertexArray.
         */
        virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const = 0;

        /**
         * \brief Gets the IndexBuffer that is used with this VertexArray.
         *
         * @return The IndexBuffers associated with this VertexArray.
         */
        virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const = 0;

        /**
         * \brief Creates a vertex array using the API specified inside of the application singleton.
         * @return A shared_ptr of the newly created VertexArray in whatever API is specified.
         */
        static std::shared_ptr<VertexArray> create();
    };

}

#endif //GEOGL_VERTEXARRAY_HPP
