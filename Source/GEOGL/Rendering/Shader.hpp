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


#ifndef GEOGL_SHADER_HPP
#define GEOGL_SHADER_HPP

namespace GEOGL{

    /**
     * \brief Represents the data types that can be uploaded to a shader
     */
    enum class GEOGL_API ShaderDataType{
        NONE = 0,
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        MAT3,
        MAT4,
        INT,
        INT2,
        INT3,
        INT4,
        BOOLEAN
    };

    /**
     * \brief Gets the size (in bytes) of each ShaderDataType
     * @param type The ShaderDataType to check
     * @return The size in bytes of the ShaderDataType
     */
    GEOGL_API uint32_t shaderDataTypeSize(ShaderDataType type);

    /**
     * \brief Represents a Shader interface. This defines all of the methods used to upload shaders.
     */
    class GEOGL_API Shader{
    public:
        virtual ~Shader() =default;

        /**
         * \brief Binds a shader to be used.
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds a shader.
         * \note A shader should generally not be unbound.
         */
        virtual void unbind() const = 0;

        /**
         * Creates a shader from source files.
         * \todo Change from vertexSrc and fragmentSrc to a more robust way. Maybe a struct with some bools describing the data? Just trying to think about vulkan in the future with SPIR-V and how we can impliment it with OpenGL.
         * \note If the shader source does not compile, there is no mechanism to catch the error.
         * This method will hard crash the program with an exit code of 2.
         *
         * @param vertexSrc The literal source for the vertex shader
         * @param fragmentSrc The literal source for the fragment shader
         * @return A shared_ptr of the newly created shader. Ready for use.
         */
        static std::shared_ptr<Shader> create(const std::string& vertexSrc, const std::string& fragmentSrc);

    };

}

#endif //GEOGL_SHADER_HPP
