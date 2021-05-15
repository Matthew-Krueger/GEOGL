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


#ifndef GEOGL_VULKANGRAPHICSCONTEXT_HPP
#define GEOGL_VULKANGRAPHICSCONTEXT_HPP

#include "../../../GEOGL/Rendering/GraphicsContext.hpp"

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace GEOGL::Platform::Vulkan{

    /**
     * \brief Implements the Graphics context for OpenGL
     */
    class GEOGL_API GraphicsContext : public GEOGL::GraphicsContext{
    public:
        explicit GraphicsContext(GLFWwindow* windowHandle, std::string appName, uint32_t versionMajor, uint32_t versionMinor, uint32_t versionPatch);
        ~GraphicsContext() override;

        void clearColor() override;

        void setViewport(const glm::ivec2& topLeftCorner, const glm::ivec2& dimensions) override;
        void setVSync(bool* vSyncStatus) override;
        void swapBuffers() override;

    private:
        void createInstance();

        std::vector<const char*> getRequiredExtensions();

        bool checkValidationLayerSupport();
    private:
        GLFWwindow* m_WindowHandle;
        std::string m_AppName;
        uint32_t m_VersionMajor, m_VersionMinor, m_VersionPatch;

        VkInstance m_Instance;

    };

}


#endif //GEOGL_VULKANGRAPHICSCONTEXT_HPP
