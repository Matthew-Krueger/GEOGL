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



#include "VulkanExtensions.hpp"

namespace GEOGL::Platform::Vulkan{

    /* function ptr for vkCreateDebugUtilsMessengerExt */
    static VkResult (*pfnvkCreateDebugUtilsMessengerEXT)
    (VkInstance,const VkDebugUtilsMessengerCreateInfoEXT*,
            const VkAllocationCallbacks*,
            VkDebugUtilsMessengerEXT*) = nullptr;

    static void (*pfnvkDestroyDebugUtilsMessengerEXT)
    (VkInstance,
            VkDebugUtilsMessengerEXT,
            const VkAllocationCallbacks *) = nullptr;

    VkResult Extensions::vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                        const VkAllocationCallbacks *pAllocator,
                                                        VkDebugUtilsMessengerEXT *pDebugMessenger) {
        /* load function if necessary */
        if(pfnvkCreateDebugUtilsMessengerEXT == nullptr){
            pfnvkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        }

        GEOGL_CORE_ASSERT(pfnvkCreateDebugUtilsMessengerEXT, "Unable to load requested vulkan function vkCreateDebugUtilsMessengerEXT");

        if(pfnvkCreateDebugUtilsMessengerEXT){
            return pfnvkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;

    }

    void Extensions::vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                         const VkAllocationCallbacks *pAllocator) {
        /* load function if necessary */
        if(pfnvkDestroyDebugUtilsMessengerEXT == nullptr){
            pfnvkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        }

        GEOGL_CORE_ASSERT(pfnvkDestroyDebugUtilsMessengerEXT, "Unable to load requested vulkan function vkDestroyDebugUtilsMessengerEXT");

        if(pfnvkCreateDebugUtilsMessengerEXT){
            return pfnvkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, pAllocator);
        }
    }
}