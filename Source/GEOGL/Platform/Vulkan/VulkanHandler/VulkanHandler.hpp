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

    /**
     * \brief Holds the queue families information to create a queue.
     */
    struct QueueFamilyIndices{
        std::optional<uint32_t> graphicsFamly;

         [[nodiscard]] inline bool isComplete() const{
            return graphicsFamly.has_value();
        }
    };

    /**
     * \brief Holds the state of the vulkan context.
     *
     * This class holds all of the functions required to interface with the vulkan context.
     * It will create and destroy everything that is necessary to properly interface with vulkan as well.
     * When using this elsewhere, always reference as a raw pointer.
     */
    class GEOGL_API_HIDDEN VulkanContext{

    public:
        // Constructor and Destructor
        VulkanContext(const char* windowTitle, const GEOGL::WindowProps& windowProps);
        ~VulkanContext();


    private:
        // Device Suitability
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool isDeviceSuitable(const VkPhysicalDevice device) const;
        uint64_t rateDeviceSuitablty(const VkPhysicalDevice device) const;

        // Required Extensions
        std::vector<const char*> getRequiredExtensions() const;

        // Queue Family Functions
        GEOGL_API_HIDDEN QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        // debug functions
        bool checkValidationLayerSupport(const std::vector<const char*>& layers) const;
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
        void enumerateExtensions() const;
        void setupDebugMessenger();

    private:
        VkInstance m_VulkanInstance;
        VkDebugUtilsMessengerEXT m_VulkanDebugMessenger;
        VkPhysicalDevice m_VulkanPhysicalDevice;
        VkDevice m_VulkanLogicalDevice;
        VkQueue m_VulkanGraphicsQueue;

    };

}

#endif //NODIFY_SCREENWRITER_VULKANHANDLER_HPP
