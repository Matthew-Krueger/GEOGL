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
#include <array>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <utility>

namespace GEOGL::Platform::OpenGL{

    static GLenum shaderTypeFromString(const std::string& type){
        GEOGL_PROFILE_FUNCTION();

        /* handle windows files. Boo windows */
        std::string svType = (type.substr(type.size()-1,1) == "\r") ? type.substr(0,type.size()-1) : type;

        if(svType == "vertex") {
            return GL_VERTEX_SHADER;
        }

        if(svType == "fragment" || svType == "frag" || svType == "pixel") {
            return GL_FRAGMENT_SHADER;
        }

        GEOGL_CORE_ASSERT_NOSTRIP(false, "Unknown shader type {}!", svType);
        return GL_INVALID_ENUM;

    }

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

    Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc, std::string name) : m_Name(std::move(name)){
        GEOGL_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexSrc;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentSrc;

        compile(shaderSource);

    }

    Shader::Shader(const std::string& folderPath){
        GEOGL_PROFILE_FUNCTION();

        /* first, read the source */
        std::string fileToLoad = folderPath+"/shader.glsl";
        GEOGL_CORE_INFO("Loading shader {}", fileToLoad);
        std::string source = readFile(fileToLoad);

        auto shaderSources = preprocess(source);

        compile(shaderSources);

        /* Get the name from the folder path */
        auto lastSlash = folderPath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash +1;
        auto count = folderPath.size() - lastSlash;

        m_Name = folderPath.substr(lastSlash, count);

    }


    Shader::~Shader() {
        GEOGL_PROFILE_FUNCTION();

        for(auto& kv : shaderIDs){
            glDetachShader(m_RendererID, kv);
        }
        for(auto& kv : shaderIDs){
            glDeleteShader(kv);
        }

        glDeleteProgram(m_RendererID);
        setBoundID(0);

    }

    void Shader::bind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        if(m_RendererID != getBoundID()) {
            glUseProgram(m_RendererID);
            setBoundID(m_RendererID);
        }

    }

    void Shader::unbind() const {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        setBoundID(0);
        glUseProgram(0);

    }

    void Shader::setInt(const char *name, int value){

        uploadUniformInt(name, value);

    }

    void Shader::setIntArray(const char * name, int* values, uint32_t count){

        uploadUniformIntArray(name, values, count);

    }

    void Shader::setFloat(const char *name, float value) {

        uploadUniformFloat(name, value);

    }

    void Shader::setFloat2(const char * name, const glm::vec2 &value) {

        uploadUniformFloat2(name, value);

    }

    void Shader::setFloat3(const char * name, const glm::vec3 &value) {

        uploadUniformFloat3(name, value);

    }

    void Shader::setFloat4(const char * name, const glm::vec4 &value) {

        uploadUniformFloat4(name, value);

    }

    void Shader::setMat4(const char * name, const glm::mat4 &value) {

        uploadUniformMat4(name, value);

    }

    void Shader::uploadUniformInt(const char *uniformName, int value) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform1i(uniformLocation, value);

    }

    void Shader::uploadUniformIntArray(const char * uniformName, int* values, uint32_t count){

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        GEOGL_CORE_ASSERT(uniformLocation != -1, "Uniform {} not found.", uniformName);

        /* now, upload the data */
        glUniform1uiv(uniformLocation, (GLsizei)count, (GLuint*) values);

    }

    void Shader::uploadUniformInt2(const char *uniformName, const glm::ivec2& vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform2iv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformInt3(const char *uniformName, const glm::ivec3& vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform3iv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformInt4(const char *uniformName, const glm::ivec4& vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform4iv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformFloat(const char *uniformName, float value) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform1f(uniformLocation, value);

    }

    void Shader::uploadUniformFloat2(const char *uniformName, const glm::vec2 &vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform2fv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformFloat3(const char *uniformName, const glm::vec3 &vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        bind();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform3fv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformFloat4(const char *uniformName, const glm::vec4 &vector) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));

    }

    void Shader::uploadUniformMat3(const char *uniformName, const glm::mat3 &matrix) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Get the uniform's location */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));

    }

    void Shader::uploadUniformMat4(const char *uniformName, const glm::mat4 &matrix) {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        /* Tried caching uniforms. It was about 15% slower. So, just query OpenGL every frame */
        int uniformLocation = glGetUniformLocation(m_RendererID, uniformName);

        /* now, upload the data */
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));

    }

    std::string Shader::readFile(const std::string& filePath){
        GEOGL_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filePath, std::ios::in | std::ios::binary);
        GEOGL_CORE_ASSERT(in, "Could not open shader file {}", filePath);
        if(!in){
            GEOGL_CORE_ERROR_NOSTRIP("Could not find shader {}", filePath);
            return "";
        }

        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], (std::streamsize)result.size());
        in.close();

        return result;

    }

    std::unordered_map<uint32_t, std::string> Shader::preprocess(const std::string& source){
        GEOGL_PROFILE_FUNCTION();

        std::unordered_map<uint32_t, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while(pos != std::string::npos){
            size_t eol = source.find_first_of('\n', pos);
            GEOGL_CORE_ASSERT_NOSTRIP(eol != std::string::npos, "Syntax error parsing shader source!");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol-begin);
            GEOGL_CORE_ASSERT_NOSTRIP(shaderTypeFromString(type) != GL_INVALID_ENUM, "Invalid shader type specification {}", type);

            size_t nextLinePos = source.find_first_not_of('\n', eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[shaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() -1 : nextLinePos));
        }

        /* Default shader, really flipping obnoxious */
        if(shaderSources.find(GL_FRAGMENT_SHADER) == shaderSources.end()){
            shaderSources[GL_FRAGMENT_SHADER] = R"(
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(1.0f,0.0f,0.5f,1.0f);
}

)";
        }

        /* Default shader, really flipping obnoxious */
        if(shaderSources.find(GL_VERTEX_SHADER) == shaderSources.end()){
            shaderSources[GL_VERTEX_SHADER] = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = vec4(a_Position.xyz,1.0f);
}

)";
        }


        return shaderSources;

    }

    void Shader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
        GEOGL_PROFILE_FUNCTION();

        GLuint programID = 0;
        /* create a program */
        {
            GEOGL_RENDERER_PROFILE_SCOPE("Create Program");
            programID = glCreateProgram();
        }

        GEOGL_CORE_ASSERT(shaderSources.size() <=2, "We only support 4 shader components at a time!");
        uint32_t glShaderIDIndex = 0;
        {
            GEOGL_PROFILE_SCOPE("Compiling shaders");

            for (auto &kv: shaderSources) {
                GLenum type = kv.first;
                const std::string &source = kv.second;

                GLuint shaderID = glCreateShader(type);

                /* Send the vertex shader source code to GL */
                const GLchar *sourceCstr = source.c_str();
                glShaderSource(shaderID, 1, &sourceCstr, nullptr);

                /* Compile the vertex shader */
                glCompileShader(shaderID);

                GLint isCompiled = 0;
                glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
                if (isCompiled == GL_FALSE) {
                    GLint maxLength = 0;
                    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

                    // The maxLength includes the NULL character
                    std::string infoLog;
                    infoLog.resize(maxLength);
                    glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog.data());

                    // We don't need the shader anymore.
                    glDeleteShader(shaderID);
                    for (auto &toDelete : shaderIDs) {
                        glDeleteShader(toDelete);
                    }

                    // Use the infoLog as you see fit.
                    GEOGL_CORE_ASSERT(false, "Unable to compile Shader: {}", infoLog);
                }

                shaderIDs[glShaderIDIndex++] = shaderID;

                /* Attach our shaders to our m_RenderID */
                glAttachShader(programID, shaderID);

            }
        }

        /* Link our m_RenderID */
        {
            GEOGL_PROFILE_SCOPE("Linking shader");
            glLinkProgram(programID);
        }

        /* Note the different functions here: glGetProgram* instead of glGetShader* */
        GLint isLinked = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE){
            GLint maxLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

            /* The maxLength includes the NULL character */
            std::string infoLog;
            infoLog.resize(maxLength);
            glGetProgramInfoLog(programID, maxLength, &maxLength, infoLog.data());

            /* We don't need the m_RenderID anymore. */
            glDeleteProgram(programID);
            for(auto& kv : shaderIDs){
                glDeleteShader(kv);
            }

            GEOGL_CORE_ASSERT(false, "Unable to Link shaders: {}", infoLog);
            return;

        }

        m_RendererID = programID;

        for(auto shaderID:shaderIDs){
            glDetachShader(m_RendererID,shaderID);
        }

    }

}