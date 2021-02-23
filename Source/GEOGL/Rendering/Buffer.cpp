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

#include "Buffer.hpp"
#include "../Application/Application.hpp"
#ifdef GEOGL_BUILD_WITH_OPENGL
#include "../Modules/Platform/OpenGL/Rendering/OpenGLBuffer.hpp"
#endif

namespace GEOGL{


    std::shared_ptr<VertexBuffer> VertexBuffer::create(const std::vector<glm::vec3> &vertices) {
        const RendererAPI& api = Application::get().getAPIManager();

        std::shared_ptr<VertexBuffer> result;
        switch(api.getRenderAPIType()){
            case API_OPENGL_DESKTOP:
#ifdef GEOGL_BUILD_WITH_OPENGL
                result.reset(new GEOGL::Platform::OpenGL::VertexBuffer(vertices));
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", apiPrettyPrint(api.getRenderAPIType()));
                return nullptr;
        }

    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(const std::vector<uint32_t> &indices) {
        const RendererAPI& api = Application::get().getAPIManager();

        std::shared_ptr<IndexBuffer> result;
        switch(api.getRenderAPIType()){
            case API_OPENGL_DESKTOP:
#ifdef GEOGL_BUILD_WITH_OPENGL
                result.reset(new GEOGL::Platform::OpenGL::IndexBuffer(indices));
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", apiPrettyPrint(api.getRenderAPIType()));
                return nullptr;
        }
    }
}
