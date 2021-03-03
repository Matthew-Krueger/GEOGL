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
#include "OpenGLShader.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace GEOGL::Platform::OpenGL{

    GLenum shaderDataTypeToOpenGLBaseType(enum ShaderDataType type){
        switch(type){
            case ShaderDataType::FLOAT:
            case ShaderDataType::FLOAT2:
            case ShaderDataType::FLOAT3:
            case ShaderDataType::FLOAT4:
            case ShaderDataType::MAT3:
            case ShaderDataType::MAT4:
                return GL_FLOAT;
            case ShaderDataType::INT:
            case ShaderDataType::INT2:
            case ShaderDataType::INT3:
            case ShaderDataType::INT4:
                return GL_INT;
            case ShaderDataType::BOOLEAN:
                return GL_BOOL;
            default:
            GEOGL_CORE_ASSERT(false, "Unknown shader DataType");
                return 0;
        }
    }

    Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {

        /* Create an empty vertex shader handle */
        m_VertexID = glCreateShader(GL_VERTEX_SHADER);

        /* Send the vertex shader source code to GL */
        const GLchar *source = vertexSrc.c_str();
        glShaderSource(m_VertexID, 1, &source, 0);

        /* Compile the vertex shader */
        glCompileShader(m_VertexID);

        GLint isCompiled = 0;
        glGetShaderiv(m_VertexID, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(m_VertexID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::string infoLog;
            infoLog.resize(maxLength);
            glGetShaderInfoLog(m_VertexID, maxLength, &maxLength, infoLog.data());

            // We don't need the shader anymore.
            glDeleteShader(m_VertexID);

            // Use the infoLog as you see fit.
            GEOGL_CORE_CRITICAL_NOSTRIP("Unable to compile Vertex Shader: {}", infoLog);

            // In this simple m_RenderID, we'll just leave
            exit(2);
        }

        /* Create an empty fragment shader handle */
        m_FragmentID = glCreateShader(GL_FRAGMENT_SHADER);

        /* Send the fragment shader source code to GL */
        source = (const GLchar *)fragmentSrc.c_str();
        glShaderSource(m_FragmentID, 1, &source, 0);

        /* Compile the fragment shader */
        glCompileShader(m_FragmentID);

        /* Ensure it compiles */
        glGetShaderiv(m_FragmentID, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE){
            GLint maxLength = 0;
            glGetShaderiv(m_FragmentID, GL_INFO_LOG_LENGTH, &maxLength);

            /* The maxLength includes the NULL character */
            std::string infoLog;
            infoLog.resize(maxLength);
            glGetShaderInfoLog(m_FragmentID, maxLength, &maxLength, infoLog.data());

            /* We don't need the shader anymore. */
            glDeleteShader(m_FragmentID);
            glDeleteShader(m_VertexID);

            /* Log compile error */
            GEOGL_CORE_CRITICAL_NOSTRIP("Unable to compile Fragment Shader: {}", infoLog);

            exit(2);
        }

        /* Vertex and fragment shaders are successfully compiled.
         * Now time to link them together into a m_RenderID.
         * Get a m_RenderID object. */
        m_RendererID = glCreateProgram();

        /* Attach our shaders to our m_RenderID */
        glAttachShader(m_RendererID, m_VertexID);
        glAttachShader(m_RendererID, m_FragmentID);

        /* Link our m_RenderID */
        glLinkProgram(m_RendererID);

        /* Note the different functions here: glGetProgram* instead of glGetShader* */
        GLint isLinked = 0;
        glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE){
            GLint maxLength = 0;
            glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

            /* The maxLength includes the NULL character */
            std::string infoLog;
            infoLog.resize(maxLength);
            glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

            /* We don't need the m_RenderID anymore. */
            glDeleteProgram(m_RendererID);
            glDeleteShader(m_RendererID);
            glDeleteShader(m_FragmentID);

            GEOGL_CORE_CRITICAL_NOSTRIP("Unable to Link Vertex and Fragment shaders: {}", infoLog);

            exit(2);

        }

        /* Always detach shaders after a successful link. */
        glDetachShader(m_RendererID, m_VertexID);
        glDetachShader(m_RendererID, m_FragmentID);

    }

    Shader::~Shader() {
        glDetachShader(m_RendererID, m_VertexID);
        glDetachShader(m_RendererID, m_FragmentID);

        glDeleteShader(m_VertexID);
        glDeleteShader(m_FragmentID);

        glDeleteProgram(m_RendererID);

    }

    void Shader::bind() const {

        glUseProgram(m_RendererID);

    }

    void Shader::unbind() const {

        glUseProgram(0);

    }

    void Shader::uploadUniformMat4(const char * uniformName, const glm::mat4 &matrix) {

        /* Tried caching uniforms. It was about 15% slower. So, just query OpenGL every frame */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));

    }

    void Shader::uploadUniformFloat4(const char *uniformName, const glm::vec4 &vector) {

        /* Tried caching uniforms. It was about 15% slower. So, just query OpenGL every frame */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));

    }

}