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
#include "Shader.hpp"
#include "RendererAPI.hpp"
#include "../Application/Application.hpp"

#ifdef GEOGL_BUILD_WITH_OPENGL
#include "../Modules/Platform/OpenGL/Rendering/OpenGLShader.hpp"
#endif

namespace GEOGL{


    std::shared_ptr<Shader> Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc) {

        const RendererAPI& api = Application::get().getAPIManager();

        std::shared_ptr<Shader> result;
        switch(api.getRenderAPIType()){
            case API_OPENGL_DESKTOP:
#ifdef GEOGL_BUILD_WITH_OPENGL
            result.reset(new GEOGL::Platform::OpenGL::Shader(vertexSrc, fragmentSrc));
            return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", apiPrettyPrint(api.getRenderAPIType()));
                return result;
        }


    }
}