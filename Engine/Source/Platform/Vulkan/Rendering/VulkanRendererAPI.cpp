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

#include "VulkanRendererAPI.hpp"

namespace GEOGL::Platform::Vulkan{


    /* Since this is the OpenGL Rendering api, we know the api is OpenGL */
    RendererAPI::RendererAPI() : GEOGL::RendererAPI(RENDERING_VULKAN_DESKTOP){

    }

    RendererAPI::~RendererAPI() {

    }

    void RendererAPI::init(){

        GEOGL_CORE_INFO("Initializing the RendererAPI");

        /* Enable blending */

        /* Log GPU information */

    }

    void RendererAPI::setClearColor(const glm::vec4 &color) {
        m_ClearColor = color;
    }

    void RendererAPI::clear() {

        GEOGL_CORE_INFO("Clearing Frame!");

    }

    void RendererAPI::drawIndexed(const Ref<VertexArray> &vertexArray) {

        /* Issue a draw call for indexed */
        /* glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr); */
        GEOGL_CORE_INFO("Issuing a draw call!");

    }
}

