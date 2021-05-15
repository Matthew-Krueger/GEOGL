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

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <utility>
#include "VulkanGraphicsContext.hpp"

namespace GEOGL::Platform::Vulkan{

    const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
    };

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    /*
     * Graphics context
     */
    GraphicsContext::GraphicsContext(GLFWwindow* windowHandle, std::string appName, uint32_t versionMajor, uint32_t versionMinor, uint32_t versionPatch) : m_WindowHandle(windowHandle),
                                                                                                 m_AppName(std::move(appName)), m_VersionMajor(versionMajor), m_VersionMinor(versionMinor), m_VersionPatch(versionPatch){

        GEOGL_CORE_INFO_NOSTRIP("Loading Vulkan Graphics Context");
        GEOGL_CORE_ASSERT(windowHandle, "Window Handle cannot be NULL.");

        createInstance();


        GEOGL_CORE_ASSERT(false, "Crashing because not built");


    }

    GraphicsContext::~GraphicsContext() {

        vkDestroyInstance(m_Instance, nullptr);

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

    void GraphicsContext::createInstance() {

        /* Check on validation layer support */
        if(enableValidationLayers && !checkValidationLayerSupport()){
            GEOGL_CORE_ASSERT_NOSTRIP(false, "Requested validation layers are not supported.");
        }

        /* Create the vulkan application */
        {

            VkApplicationInfo applicationInfo{};
            applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            applicationInfo.pApplicationName = m_AppName.c_str();
            applicationInfo.applicationVersion = VK_MAKE_VERSION(m_VersionMajor, m_VersionMinor, m_VersionPatch);
            applicationInfo.pEngineName = GEOGL_ENGINE_NAME_NO_VERSION;
            applicationInfo.engineVersion = VK_MAKE_VERSION(GEOGL_VERSION_MAJOR, GEOGL_VERSION_MINOR, GEOGL_VERSION_PATCH);
            applicationInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &applicationInfo;

            auto requiredExtensions = getRequiredExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();

            /* handle validation layers */
            if(enableValidationLayers){
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }else {
                createInfo.enabledLayerCount = 0;
            }

            VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
            GEOGL_CORE_ASSERT_NOSTRIP(result == VK_SUCCESS, "Error creating vulkan instance. Code {}", result);

        }



    }

    std::vector<const char *> GraphicsContext::getRequiredExtensions() {

        std::vector<const char *> requiredExtensions;
        {
            uint32_t glfwExtensionCount = 0;
            const char **glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            requiredExtensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
        }

        if(enableValidationLayers){
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        /* Output a list of required extensions */
        {
            std::stringstream output;
            output << GEOGL_FORMAT("{} requests {} instance extensions:\n", GEOGL_ENGINE_NAME_NO_VERSION, requiredExtensions.size());
            for(auto& requestedExtension:requiredExtensions){
                output << '\t' << requestedExtension << '\n';
            }

            GEOGL_CORE_INFO_NOSTRIP(output.str());
        }

        /* Obtain list of available extensions */
        std::vector<VkExtensionProperties> availableExtensions;

        {
            /* get the number of extensions and reserve them */
            std::stringstream output;
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            availableExtensions.resize(extensionCount);

            /* Query vulkan for the extensions */
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

            output << "Vulkan reports " << availableExtensions.size() << " available instance extensions. These are:\n";

            for(const auto& availableExtension: availableExtensions){
                output << '\t' << availableExtension.extensionName << '\n';
            }

            GEOGL_CORE_INFO_NOSTRIP(output.str());

        }

        /* ensure all required extensions are present */
        for(const char * requiredExtension : requiredExtensions){
            bool extensionFound = false;

            for(const auto& availableExtension : availableExtensions){

                if(strcmp(requiredExtension, availableExtension.extensionName) == 0){
                    extensionFound = true;
                    break;
                }

            }

            GEOGL_CORE_ASSERT_NOSTRIP(extensionFound, "Extension {} was not found.", requiredExtension);

        }


        return requiredExtensions;

    }

    bool GraphicsContext::checkValidationLayerSupport() {

        /* Get available instance validation layers */
        std::vector<VkLayerProperties> availableLayers;
        {
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
            availableLayers.resize(layerCount);

            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        }

        /* output validation layers to console */
        {
            std::stringstream output;
            output << GEOGL_FORMAT("{} requests {} instance validation layers:\n", GEOGL_ENGINE_NAME_NO_VERSION, validationLayers.size());
            for(auto& layerName: validationLayers){
                output << '\t' << layerName << '\n';
            }
            GEOGL_CORE_INFO_NOSTRIP(output.str());
            output.str(std::string());
            output.clear();

            output << GEOGL_FORMAT("Vulkan reports that {} instance validation layers are supported:\n", availableLayers.size());
            for(auto& layerProperties : availableLayers){
                output << '\t' << layerProperties.layerName << "\tDescription: " << layerProperties.description << '\n';
            }

            GEOGL_CORE_INFO_NOSTRIP(output.str());
        }

        /* Check for necessary validation layers. */
        for(const char * layerName : validationLayers){
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers){

                if(strcmp(layerName, layerProperties.layerName) == 0){
                    layerFound = true;
                    break;
                }

            }

            GEOGL_CORE_ASSERT_NOSTRIP(layerFound, "Required validation layer {} not found!", layerName);

        }


        return true;
    }

}
