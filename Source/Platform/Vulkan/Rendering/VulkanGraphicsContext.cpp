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

#include "VulkanGraphicsContext.hpp"
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

#include <GLFW/glfw3.h>
#include "VulkanGraphicsContext.hpp"

namespace GEOGL::Platform::Vulkan{

    /*
     * Graphics context
     */
    GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle){

        GEOGL_CORE_INFO("TODO Loading Vulkan Graphics Context");
        GEOGL_CORE_ASSERT(windowHandle, "Window Handle cannot be NULL.");

    }

    GraphicsContext::~GraphicsContext() {

    }

    void GraphicsContext::setViewport(const glm::ivec2& topLeftCorner, const glm::ivec2& dimensions){

        GEOGL_CORE_INFO("TODO Setting viewport");

    }

    void GraphicsContext::clearColor(){

        GEOGL_CORE_INFO("TODO Clearing Color");

    }

    void GraphicsContext::setVSync(bool* vSyncStatus){

        GEOGL_CORE_INFO("TODO Setting vsync status to {}", *vSyncStatus);

    }

    void GraphicsContext::swapBuffers() {

        GEOGL_CORE_INFO("TODO Swapchain swap buffers");

    }

}
