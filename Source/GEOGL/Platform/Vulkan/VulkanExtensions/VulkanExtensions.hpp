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

#ifndef GEOGL_VULKANEXTENSIONS_HPP
#define GEOGL_VULKANEXTENSIONS_HPP

#include <vulkan/vulkan.h>

namespace GEOGL{

    /**
     * Holds information regarding extensions loaded for vulkan
     */
    namespace VulkanExtensions {

        /**
         * \brief Debug callback for validation layers
         * @param messageSeverity The severity of the message
         * @param messageType The type of the message
         * @param pCallbackData The data of the message
         * @param pUserData The userdata of the message
         * @return No clue what it is, but it always returns VK_FALSE
         */
        VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                     VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                     const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                     void *pUserData);

        /**
         * \brief Creates a debug messenger (and loads it from the DLL).
         * @param instance The instance to attach it to
         * @param pCreateInfo The Creation info of the debug messenger
         * @param pAllocator The allocator function
         * @param pDebugMessenger The debugMessenger pointer
         * @return VK_SUCCESS if successful, false otherwise
         */
        VkResult
        createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                     const VkAllocationCallbacks *pAllocator,
                                     VkDebugUtilsMessengerEXT *pDebugMessenger);

        void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks *pAllocator);

    }

}

#endif //NODIFY_SCREENWRITER_VULKANEXTENSIONS_HPP
