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


#include "VertexArray.hpp"

#include <memory>
#include "../Application/Application.hpp"
#include "Renderer.hpp"

#if GEOGL_BUILD_WITH_OPENGL == 1
#include "../../Platform/OpenGL/Rendering/OpenGLVertexArray.hpp"
#endif

namespace GEOGL{

    uint32_t VertexArray::s_BoundVAOID = 0;

    Ref<VertexArray> VertexArray::create() {
        GEOGL_PROFILE_FUNCTION();

        const auto renderer = Renderer::getRendererAPI();

        Ref<VertexArray> result;
        switch(renderer->getRenderingAPI()){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
#if GEOGL_BUILD_WITH_OPENGL == 1
                result = createRef<GEOGL::Platform::OpenGL::VertexArray>();
                return result;
#else
                GEOGL_CORE_CRITICAL("Platform OpenGL Slected but not supported.");
#endif
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP("Unable to create a {} shader. Unhandled path.", RendererAPI::getRenderingAPIName(renderer->getRenderingAPI()));
                return result;
        }
    }
}
