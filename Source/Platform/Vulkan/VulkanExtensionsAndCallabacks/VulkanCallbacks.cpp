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

#include "VulkanCallbacks.hpp"

namespace GEOGL::Platform::Vulkan {
    VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverityFlagBitsExt,
                                                       VkDebugUtilsMessageTypeFlagsEXT debugUtilsMessageTypeFlagBitsExt,
                                                 const VkDebugUtilsMessengerCallbackDataEXT *debugUtilsMessengerCallbackDataExt,
                                                 void *userData) {

        /* Kick out VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT for performance */
        if(messageSeverityFlagBitsExt <= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
            return VK_FALSE;
        }

        /* Format the output */
        std::stringstream output;
        {

            output << "Vulkan ";
            /* decode message type */
            switch (debugUtilsMessageTypeFlagBitsExt) {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                    output << "General Information";
                    return VK_FALSE;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                    output << "Validation Error (Standard Violation or Programming Mistake)";
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    output << "Potential Performance Issue";
                default:
                    output << "Unknown Error";
            }
            output << ":\n";

            /* decode the message */
            output << "\tObject Count: " << debugUtilsMessengerCallbackDataExt->objectCount << '\n';
            output << "\tObjects: " << '\n';
            for (size_t position = 0; position < debugUtilsMessengerCallbackDataExt->objectCount; ++position) {
                const char* objectName = debugUtilsMessengerCallbackDataExt->pObjects[position].pObjectName;
                output << "\t\t" << ((objectName) ? objectName : "Unnamed Object") << '\n';
            }
            output << "\tMessage ID Name: " << debugUtilsMessengerCallbackDataExt->pMessageIdName << '\n';
            output << "\tMessage: " << debugUtilsMessengerCallbackDataExt->pMessage << '\n';

        }

        switch(messageSeverityFlagBitsExt){
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                return VK_FALSE;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                GEOGL_CORE_INFO(output.str());
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                GEOGL_CORE_WARN_NOSTRIP(output.str());
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                GEOGL_CORE_ERROR_NOSTRIP(output.str());
            default:
                break;
        }

        return VK_FALSE;

    }

}