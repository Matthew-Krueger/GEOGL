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

#include <memory>
#include "Shader.hpp"
#include "RendererAPI.hpp"
#include "../Application/Application.hpp"
#include "Renderer.hpp"

#if GEOGL_BUILD_WITH_OPENGL == 1
#include "../../Platform/OpenGL/Rendering/OpenGLShader.hpp"
#endif

namespace GEOGL{

    uint32_t Shader::s_BoundShaderID = 0;

    uint32_t shaderDataTypeSize(ShaderDataType type){
        switch(type){
            case ShaderDataType::FLOAT:
                return 4;
            case ShaderDataType::FLOAT2:
                return 4*2;
            case ShaderDataType::FLOAT3:
                return 4*3;
            case ShaderDataType::FLOAT4:
                return 4*4;
            case ShaderDataType::MAT3:
                return 4*3*3;
            case ShaderDataType::MAT4:
                return 4*4*4;
            case ShaderDataType::INT:
                return 4;
            case ShaderDataType::INT2:
                return 4*2;
            case ShaderDataType::INT3:
                return 4*3;
            case ShaderDataType::INT4:
                return 4*4;
            case ShaderDataType::BOOLEAN:
                return 1;
            default:
            GEOGL_CORE_ASSERT(false, "Unknown shader DataType");
                return 0;
        }
    }

    Ref<Shader> Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc, const std::string& name) {
        GEOGL_PROFILE_FUNCTION();

        const auto renderer = Renderer::getRendererAPI();

        Ref<Shader> result;
        switch(renderer->getRenderingAPI()){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
#if GEOGL_BUILD_WITH_OPENGL == 1
            result = createRef<GEOGL::Platform::OpenGL::Shader>(vertexSrc, fragmentSrc, name);
            return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", RendererAPI::getRenderingAPIName(renderer->getRenderingAPI()));
                return result;
        }


    }

    Ref<Shader> Shader::create(const std::string& folderPath) {
        GEOGL_PROFILE_FUNCTION();

        const auto renderer = Renderer::getRendererAPI();

        Ref<Shader> result;
        switch(renderer->getRenderingAPI()){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
#if GEOGL_BUILD_WITH_OPENGL == 1
                result = createRef<GEOGL::Platform::OpenGL::Shader>(folderPath);
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", RendererAPI::getRenderingAPIName(renderer->getRenderingAPI()));
                return result;
        }


    }

    void ShaderLibrary::add(const Ref <Shader> &shader) {
        GEOGL_PROFILE_FUNCTION();

        auto& name = shader->getName();
        add(name, shader);

    }

    void ShaderLibrary::add(const std::string &customName, const Ref <Shader> &shader) {
        GEOGL_PROFILE_FUNCTION();

        GEOGL_CORE_ASSERT(m_Shaders.find(customName) == m_Shaders.end(), "Shader {} is already in the library!", customName);
        m_Shaders[customName] = shader;

    }

    std::pair<std::string, Ref<Shader>> ShaderLibrary::load(const std::string &folderPath) {
        GEOGL_PROFILE_FUNCTION();

        auto shader = Shader::create(folderPath);
        add(shader);
        return std::make_pair(shader->getName(),shader);

    }

    std::pair<std::string, Ref<Shader>> ShaderLibrary::load(const std::string &name, const std::string &folderPath) {
        GEOGL_PROFILE_FUNCTION();

        auto shader = Shader::create(folderPath);
        add(name, shader);
        return std::make_pair(name, shader);

    }

    Ref <Shader> ShaderLibrary::get(const std::string &name) {
        GEOGL_PROFILE_FUNCTION();

        bool found = m_Shaders.find(name) != m_Shaders.end();
        GEOGL_CORE_ASSERT(found, "Shader {} is not in the library!", name);
        if(found)
            return m_Shaders[name];
        return nullptr; /* Return nullptr if not found. Should crash */

    }

}