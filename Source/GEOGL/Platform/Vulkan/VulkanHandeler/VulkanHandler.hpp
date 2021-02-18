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

#ifndef GEOGL_VULKANHANDLER_HPP
#define GEOGL_VULKANHANDLER_HPP

#include "../../../IO/Window.hpp"
#include <vulkan/vulkan.h>
#include <optional>

namespace GEOGL::VulkanHandler{


    struct QueueFamilyIndices{
        std::optional<uint32_t> graphicsFamly;
         [[nodiscard]] inline bool isComplete() const{
            return graphicsFamly.has_value();
        }
    };

    class GEOGL_API_HIDDEN VulkanContext{

    public:
        // Constructor and Destructor
        VulkanContext(const char* windowTitle, const GEOGL::WindowProps& windowProps);
        ~VulkanContext();

        // Device Suitability
        void pickPhysicalDevice();
        void createLogicalDevice();

        // debug functions
        void setupDebugMessenger();

    private:
        VkInstance m_VulkanInstance;
        VkDebugUtilsMessengerEXT m_VulkanDebugMessenger;
        VkPhysicalDevice m_VulkanPhysicalDevice;
        VkDevice m_VulkanLogicalDevice;
        VkQueue m_VulkanGraphicsQueue;

    };

    //GEOGL_API_HIDDEN void createInstance(VulkanContext* vulkanContext, const char* windowTitle, const GEOGL::WindowProps& windowProps);
    //GEOGL_API_HIDDEN void destroyInstance(VulkanContext* vulkanContext);

    GEOGL_API_HIDDEN std::vector<const char*> getRequiredExtensions();

    // Device Suitability Functions
    //GEOGL_API_HIDDEN void pickPhysicalDevice(VulkanContext* vulkanContext);
    GEOGL_API_HIDDEN bool isDeviceSuitable(VkPhysicalDevice device);
    GEOGL_API_HIDDEN uint64_t rateDeviceSuitablty(VkPhysicalDevice device);

    // Queue Family Functions
    GEOGL_API_HIDDEN QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    // DEBUG FUNCTIONS
    GEOGL_API_HIDDEN bool checkValidationLayerSupport(const std::vector<const char*>& layers);
    GEOGL_API_HIDDEN void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    GEOGL_API_HIDDEN void enumerateExtensions();

}

#endif //NODIFY_SCREENWRITER_VULKANHANDLER_HPP
