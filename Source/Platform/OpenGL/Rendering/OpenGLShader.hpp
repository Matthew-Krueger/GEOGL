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

#ifndef GEOGL_OPENGLSHADER_HPP
#define GEOGL_OPENGLSHADER_HPP

#include <glad/glad.h>
#include "../../GEOGL/Rendering/Shader.hpp"

namespace GEOGL::Platform::OpenGL{

    GEOGL_API GLenum shaderDataTypeToOpenGLBaseType(enum ShaderDataType type);

    class GEOGL_API Shader : public GEOGL::Shader{
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        Shader(const std::string& folderPath);
        ~Shader() override;

        void bind() const override;
        void unbind() const override;

        void uploadUniformInt(const char * uniformName, int value);
        void uploadUniformInt2(const char * uniformName, const glm::ivec2& vector);
        void uploadUniformInt3(const char * uniformName, const glm::ivec3& vector);
        void uploadUniformInt4(const char * uniformName, const glm::ivec4& vector);

        void uploadUniformFloat(const char * uniformName, float matrix);
        void uploadUniformFloat2(const char* uniformName, const glm::vec2& vector);
        void uploadUniformFloat3(const char * uniformName, const glm::vec3& vector);
        void uploadUniformFloat4(const char* uniformName, const glm::vec4& vector);

        void uploadUniformMat3(const char * uniformName, const glm::mat3& matrix);
        void uploadUniformMat4(const char * uniformName, const glm::mat4& matrix);
    private:

        std::string readFile(const std::string& filePath);
        std::unordered_map<uint32_t, std::string> preprocess(const std::string& source);
        void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        uint32_t m_RendererID;
        std::vector<uint32_t> m_programComponentIDs;
    };

}

#endif //GEOGL_OPENGLSHADER_HPP
