//
// Created by matth on 2/16/21.
//

#include <GLFW/glfw3.h>
#include "VulkanHandler.hpp"
#include "../VulkanExtensions/VulkanExtensions.hpp"

namespace GEOGL::VulkanHandler{

#ifndef NDEBUG
        bool enableValidationLayers = true;
#else
        bool enableValidationLayers = false;
#endif

        static std::vector<const char*> validationLayers;

        void createInstance(VkInstance *vulkanInstanceHandle, VkPhysicalDevice* physicalDeviceHandle, const char* windowTitle, const GEOGL::WindowProps& windowProps, VkDebugUtilsMessengerEXT* vulkanDebugMessengerHandle) {

            validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
            };

            /* Check validation layer support */
            {
                if(enableValidationLayers && !checkValidationLayerSupport(validationLayers)){
                    GEOGL_ASSERT(false, "Validation layers requested, but not supported.");
                }
            }

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = windowTitle;
            appInfo.applicationVersion = VK_MAKE_VERSION(windowProps.versionMajor, windowProps.versionMinor, windowProps.versionPatch);
            appInfo.pEngineName = GEOGL_ENGINE_NAME_NO_VERSION;
            appInfo.engineVersion = VK_MAKE_VERSION(GEOGL_VERSION_MAJOR, GEOGL_VERSION_MINOR, GEOGL_VERSION_PATCH);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            auto extensions = getRequiredExtensions();

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = extensions.size();
            createInfo.ppEnabledExtensionNames = extensions.data();

            /* Check for validation layers, and enable if requested */
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if(enableValidationLayers){
                GEOGL_CORE_INFO_NOSTRIP("Validation Layers requested");
                uint16_t count = 0;
                for(auto& layer:validationLayers){
                    ++count;
                    GEOGL_CORE_INFO_NOSTRIP("Validation layer #{}: {}", count, layer);
                }
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();

                VulkanHandler::populateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

            }else{
                GEOGL_CORE_INFO_NOSTRIP("No validation layers requested");
                createInfo.enabledLayerCount = 0;
                createInfo.pNext = nullptr;
            }

            /* Create the instance */
            VkResult createInstanceResult = vkCreateInstance(&createInfo, nullptr, vulkanInstanceHandle);
            GEOGL_CORE_ASSERT_NOSTRIP(createInstanceResult == VK_SUCCESS, "Unable to create Vulkan Instance. Error {}.", createInstanceResult);
            GEOGL_CORE_INFO("Vulkan Instance Created.");

            if(enableValidationLayers) setupDebugMessenger(vulkanInstanceHandle, vulkanDebugMessengerHandle);

            /* pick physical device */
            pickPhysicalDevice(vulkanInstanceHandle, physicalDeviceHandle);

        }

        std::vector<const char*> getRequiredExtensions(){

            /* Enumerate Extensions from GLFW */
            uint32_t glfwExtensionCount = 0;
            const char ** glfwExtensions;
            {
                glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
                GEOGL_CORE_INFO("GLFW reports {} extensions for Vulkan.", glfwExtensionCount);
#ifndef NDEBUG
                for(int i=0; i<glfwExtensionCount; ++i){
                    GEOGL_CORE_INFO("GLFW Extension #{}: {}", i+1, glfwExtensions[i]);
                }
#endif
            }

            std::vector<const char *> extensions(glfwExtensions, glfwExtensions+glfwExtensionCount);

            if(enableValidationLayers){
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        void pickPhysicalDevice(VkInstance* vulkanInstanceHandle, VkPhysicalDevice* vkPhysicalDeviceHandle){

            GEOGL_CORE_INFO("Picking a Physical Device.");
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(*vulkanInstanceHandle, &deviceCount, nullptr);
            GEOGL_CORE_ASSERT_NOSTRIP(deviceCount, "No Vulkan devices were found.");
            GEOGL_CORE_INFO("Vulkan reports that {} Physical Devices were found.", deviceCount);

            /* ask vulkan about the devices. */
            std::multimap<uint64_t, VkPhysicalDevice> candidates;
            {
                std::vector<VkPhysicalDevice> devices(deviceCount);
                vkEnumeratePhysicalDevices(*vulkanInstanceHandle, &deviceCount, devices.data());

                for(const auto& device:devices){
                    uint64_t score = rateDeviceSuitablty(device);
                    candidates.insert(std::make_pair(score,device));
                }

                // check if the best candidates are suitable at all.
                GEOGL_ASSERT_NOSTRIP(isDeviceSuitable(candidates.rbegin()->second) && candidates.rbegin()->first>0,
                        "Failed to find a suitable GPU.");
                *vkPhysicalDeviceHandle = candidates.rbegin()->second;


            }



            GEOGL_ASSERT_NOSTRIP(*vkPhysicalDeviceHandle != VK_NULL_HANDLE, "Failed to get handle for Vulkan Physical Device");

        }

        bool isDeviceSuitable(VkPhysicalDevice device){

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            /* Check suitability with a water flag type thing */
            bool suitableFlag = true;
            if(!GEOGL_ENABLE_IGPU){
                if(deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    suitableFlag = false;
            }
            if(!deviceFeatures.geometryShader){
                suitableFlag = false;
            }

            GEOGL_CORE_INFO("Vulkan reports that {} is {}suitable.", deviceProperties.deviceName, (!suitableFlag) ? std::string("not ") : std::string(""));


            return suitableFlag;

        }

        uint64_t rateDeviceSuitablty(VkPhysicalDevice device){

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(device, &deviceProperties);

            VkPhysicalDeviceFeatures deviceFeatures;
            vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

            int score = 0;

            /* try to always prefer discrete gpus */
            if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                score += 8192;
            }

            score += deviceProperties.limits.maxImageDimension2D;

            /* Blacklist no geometry shader gpus */
            if(!deviceFeatures.geometryShader){
                return 0;
            }

            GEOGL_CORE_INFO("GPU {} has a score of {}", deviceProperties.deviceName, score);

            return score;

        }

        bool checkValidationLayerSupport(const std::vector<const char*>& layers){

            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            for(const char* layerName : layers){
                bool layerFound = false;

                for(const auto& layerProperties : availableLayers){
                    if(strcmp(layerName, layerProperties.layerName) == 0){
                        layerFound = true;
                        break;
                    }
                }

                if(!layerFound)
                    return false;

            }

            return true;

        }

        void setupDebugMessenger(VkInstance* vulkanInstanceHandle, VkDebugUtilsMessengerEXT* vulkanDebugMessengerHandle) {
            if (enableValidationLayers) {

                VkDebugUtilsMessengerCreateInfoEXT createInfo{};
                populateDebugMessengerCreateInfo(createInfo);
                VkResult createDebugUtilsResult = VulkanExtensions::createDebugUtilsMessengerEXT(*vulkanInstanceHandle, &createInfo, nullptr,
                                                                               vulkanDebugMessengerHandle);
                GEOGL_ASSERT_NOSTRIP(createDebugUtilsResult == VK_SUCCESS,
                                     "Unable to create Debug Messenger. Error #{}", createDebugUtilsResult);

            }
        }

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {

            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = VulkanExtensions::debugCallback;
            createInfo.pUserData = nullptr;

        }

        void enumerateExtensions(){
            GEOGL_CORE_INFO("Asking Vulkan how many extensions are supported");
            uint32_t extensionCount =0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            GEOGL_CORE_INFO("Vulkan reports {} extensions available.", extensionCount);

            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            uint16_t count = 0;
            for(const auto& extension:extensions){
                ++count;
                GEOGL_CORE_INFO("Vulkan Extension #{}: {}", count, extension.extensionName);
            }

        }

    }