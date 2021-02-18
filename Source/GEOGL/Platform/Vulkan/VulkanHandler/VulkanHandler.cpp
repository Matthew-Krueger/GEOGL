//
// Created by matth on 2/16/21.
//

#define GLFW_INCLUDE_VULKAN
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



    // ///////////////////////////////////////////////
    //               Public Functions               //
    // ///////////////////////////////////////////////

    VulkanContext::VulkanContext(const char* windowTitle, const GEOGL::WindowProps& windowProps) {

        validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        /* Check validation layer support */
        {
            if(enableValidationLayers && !checkValidationLayerSupport(validationLayers)){
                GEOGL_CORE_ASSERT(false, "Validation layers requested, but not supported.");
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

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;

        }else{
            GEOGL_CORE_INFO_NOSTRIP("No validation layers requested");
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        /* Create the instance */
        VkResult createInstanceResult = vkCreateInstance(&createInfo, nullptr, &m_VulkanInstance);
        GEOGL_CORE_ASSERT_NOSTRIP(createInstanceResult == VK_SUCCESS, "Unable to create Vulkan Instance. Error {}.", createInstanceResult);
        GEOGL_CORE_INFO("Vulkan Instance Created.");

        if(enableValidationLayers) setupDebugMessenger();

        /* pick physical device */
        pickPhysicalDevice();

    }

    VulkanContext::~VulkanContext(){

        VulkanExtensions::destroyDebugUtilsMessengerEXT(m_VulkanInstance, m_VulkanDebugMessenger, nullptr);

        /* Destroy Device */
        vkDestroyDevice(m_VulkanLogicalDevice, nullptr);

        /* Destroy Instance */
        vkDestroyInstance(m_VulkanInstance, nullptr);

    }



    // ///////////////////////////////////////////////
    //              Private Functions               //
    // ///////////////////////////////////////////////

    void VulkanContext::pickPhysicalDevice(){

        GEOGL_CORE_INFO("Picking a Physical Device.");
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, nullptr);
        GEOGL_CORE_ASSERT_NOSTRIP(deviceCount, "No Vulkan devices were found.");
        GEOGL_CORE_INFO("Vulkan reports that {} Physical Devices were found.", deviceCount);

        /* ask vulkan about the devices. */
        std::multimap<uint64_t, VkPhysicalDevice> candidates;
        {
            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(m_VulkanInstance, &deviceCount, devices.data());

            for(const auto& device:devices){
                uint64_t score = rateDeviceSuitablty(device);
                candidates.insert(std::make_pair(score,device));
            }

            // check if the best candidates are suitable at all.
            GEOGL_CORE_ASSERT_NOSTRIP(isDeviceSuitable(candidates.rbegin()->second) && candidates.rbegin()->first>0,
                                 "Failed to find a suitable GPU.");
            m_VulkanPhysicalDevice = candidates.rbegin()->second;


        }

        GEOGL_CORE_ASSERT_NOSTRIP(m_VulkanPhysicalDevice != VK_NULL_HANDLE, "Failed to get handle for Vulkan Physical Device");
        GEOGL_CORE_INFO("Now that a physical device is created, create a logical device");

        createLogicalDevice();

    }

    void VulkanContext::createLogicalDevice(){

        GEOGL_CORE_INFO("Creating logical device");

        QueueFamilyIndices indices = findQueueFamilies(m_VulkanPhysicalDevice);
        GEOGL_CORE_ASSERT(indices.isComplete(), "Cannot find required queue family indices when creating logical device");

        {

            float queuePriority = 1.0f;

            /* Info on the queues */
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamly.value();
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            /* Info on the features we want */
            VkPhysicalDeviceFeatures deviceFeatures{};

            /* Give the parameters to the create info struct, including the
             * queueCreateInfo */
            VkDeviceCreateInfo deviceCreateInfo{};
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            /* queues */
            deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
            deviceCreateInfo.queueCreateInfoCount = 1;

            /* features */
            deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

            /* Extensnions */
            deviceCreateInfo.enabledExtensionCount = 0;

            /* validation layers */
            deviceCreateInfo.enabledLayerCount = 0;
            if(enableValidationLayers){
                deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
            }

            VkResult physicalDeviceCreateResult = vkCreateDevice(m_VulkanPhysicalDevice, &deviceCreateInfo, nullptr, &m_VulkanLogicalDevice);
            GEOGL_CORE_ASSERT_NOSTRIP(physicalDeviceCreateResult == VK_SUCCESS, "Failed to create a Logical device, error {}.", (int)physicalDeviceCreateResult);

        }

        /* Now, since the device is created, we can get the queue from it */
        vkGetDeviceQueue(m_VulkanLogicalDevice, indices.graphicsFamly.value(), 0, &m_VulkanGraphicsQueue);
        GEOGL_CORE_INFO("Successfully created a logical device and got the queues needed.");

    }

    bool VulkanContext::isDeviceSuitable(const VkPhysicalDevice device) const{

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

        QueueFamilyIndices indices = findQueueFamilies(device);
        if(!indices.isComplete())
            suitableFlag = false;


        GEOGL_CORE_INFO("Vulkan reports that {} is {}suitable.", deviceProperties.deviceName, (!suitableFlag) ? std::string("not ") : std::string(""));


        return suitableFlag;

    }

    uint64_t VulkanContext::rateDeviceSuitablty(const VkPhysicalDevice device) const{

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

    std::vector<const char*> VulkanContext::getRequiredExtensions() const{

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

    QueueFamilyIndices VulkanContext::findQueueFamilies(VkPhysicalDevice device) const{

        QueueFamilyIndices indices;

        /* Query vulkan for queue families */
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        unsigned int i=0;
        for(const auto& queueFamily:queueFamilies){
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamly = i;


            if(indices.isComplete())
                break;

            ++i;
        }


        return indices;

    }

    bool VulkanContext::checkValidationLayerSupport(const std::vector<const char*>& layers) const{

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

    void VulkanContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) const{

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = VulkanExtensions::debugCallback;
        createInfo.pUserData = nullptr;

    }

    void VulkanContext::enumerateExtensions() const{
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


    void VulkanContext::setupDebugMessenger(){
        if (enableValidationLayers) {

            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            populateDebugMessengerCreateInfo(createInfo);
            VkResult createDebugUtilsResult = VulkanExtensions::createDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr,
                                                                                             &m_VulkanDebugMessenger);
            GEOGL_CORE_ASSERT_NOSTRIP(createDebugUtilsResult == VK_SUCCESS,
                                 "Unable to create Debug Messenger. Error #{}", createDebugUtilsResult);

        }
    }

}