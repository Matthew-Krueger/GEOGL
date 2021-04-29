//
// Created by matth on 2/16/21.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <set>
#include "Context.hpp"
#include "../VulkanExtensions/Extensions.hpp"
#include "../../../../Application/Application.hpp"

namespace GEOGL::Platform::Vulkan{

#ifndef NDEBUG
        bool enableValidationLayers = true;
#else
        bool enableValidationLayers = false;
#endif

    static std::vector<const char*> validationLayers;
    static std::vector<const char*> requiredDeviceExtensions;



    // ///////////////////////////////////////////////
    //               Public Functions               //
    // ///////////////////////////////////////////////

    Context::Context(const char* windowTitle, GLFWwindow* window, const GEOGL::WindowProps& windowProps) {

        validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        requiredDeviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

        /* Create a surface */
        createSurface(window);

        /* pick physical device */
        pickPhysicalDevice();

        createSwapChain(window);

        createImageViews();

    }

    Context::~Context(){

        for(auto imageView:m_VulkanSwapChainImageViews){
            vkDestroyImageView(m_VulkanLogicalDevice, imageView, nullptr);
        }

        vkDestroySwapchainKHR(m_VulkanLogicalDevice, m_VulkanSwapchain, nullptr);

        /* Destroy Device */
        vkDestroyDevice(m_VulkanLogicalDevice, nullptr);

        /* Destory surface */
        vkDestroySurfaceKHR(m_VulkanInstance, m_VulkanSurface, nullptr);

        Extensions::destroyDebugUtilsMessengerEXT(m_VulkanInstance, m_VulkanDebugMessenger, nullptr);

        /* Destroy Instance */
        vkDestroyInstance(m_VulkanInstance, nullptr);

    }



    // ///////////////////////////////////////////////
    //              Private Functions               //
    // ///////////////////////////////////////////////

    void Context::createSurface(GLFWwindow* window){

        GEOGL_CORE_INFO("Creating vkSurfaceKHR with GLFW.");
        VkResult result = glfwCreateWindowSurface(m_VulkanInstance, window, nullptr, &m_VulkanSurface);
        GEOGL_CORE_ASSERT_NOSTRIP(result == VK_SUCCESS, "Unable to create surface for vulkan rendering. Code {}", result);
        GEOGL_CORE_INFO("vkSurfaceKHR created!");

    }

    VkSurfaceFormatKHR Context::chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats){

        for(const auto& availableFormat: availableFormats){
            if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return availableFormat;
        }

        return availableFormats[0];

    }

    VkPresentModeKHR Context::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){

        for(const auto& availablePresentMode : availablePresentModes){
            if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR){
                GEOGL_CORE_INFO("Vulkan Reports that VK_PRESENT_MODE_MAILOX_KHR is supported.");
                return availablePresentMode;
            }
        }
        GEOGL_CORE_INFO("Vulkan Reports that VK_PRESENT_MODE_MAILBOX_KHR is NOT supported. Defaulting to FIFO.");

        return VK_PRESENT_MODE_MAILBOX_KHR;

    }

    VkExtent2D Context::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window){

        if(capabilities.currentExtent.width != UINT32_MAX){
            return capabilities.currentExtent;
        }else{
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width,
                                          std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height,
                                           std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;

        }

    }

    void Context::createSwapChain(GLFWwindow *window){

        GEOGL_CORE_INFO("Creating Swapchain");

        SwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(m_VulkanPhysicalDevice);

        VkSurfaceFormatKHR surfaceFormatKhr = chooseSwapchainSurfaceFormat(swapChainSupportDetails.formats);
        VkPresentModeKHR presentModeKhr = chooseSwapPresentMode(swapChainSupportDetails.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupportDetails.capabilities, window);

        uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount;
        GEOGL_CORE_INFO("Vulkan reports that we must use a minimum of {} images in the swapchain. Using {} instead.", imageCount, imageCount+1);
        ++imageCount;

        if(swapChainSupportDetails.capabilities.maxImageCount>0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount){
            imageCount = swapChainSupportDetails.capabilities.maxImageCount;
            GEOGL_CORE_INFO("Vulkan reports that we can only use a max of {} images. Using that.", imageCount);
        }

        /* Create the swapchain */
        {

            QueueFamilyIndices indices = findQueueFamilies(m_VulkanPhysicalDevice);
            GEOGL_CORE_ASSERT_NOSTRIP(indices.isComplete(), "Cannot find all queue families when creating swapchain.");
            uint32_t queueFamilyIndices[] = {indices.graphicsFamly.value(), indices.graphicsFamly.value()};

            VkSwapchainCreateInfoKHR createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface = m_VulkanSurface;

            createInfo.minImageCount = imageCount;
            createInfo.imageFormat = surfaceFormatKhr.format;
            createInfo.imageColorSpace = surfaceFormatKhr.colorSpace;
            createInfo.imageExtent = extent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            if(indices.graphicsFamly != indices.presentFamily){
                createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }else{
                createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                createInfo.queueFamilyIndexCount = 0;
                createInfo.pQueueFamilyIndices = nullptr;
            }

            createInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;
            createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode = presentModeKhr;
            createInfo.clipped = VK_TRUE;
            createInfo.oldSwapchain = VK_NULL_HANDLE;

            VkResult result = vkCreateSwapchainKHR(m_VulkanLogicalDevice, &createInfo, nullptr, &m_VulkanSwapchain);
            GEOGL_CORE_ASSERT_NOSTRIP(result == VK_SUCCESS, "Unable to create VkSwapchainKHR, error {}.", result);

        }

        /* Now that the swapchain is created, we can retrieve the handles to
         * the VkImages */
        vkGetSwapchainImagesKHR(m_VulkanLogicalDevice, m_VulkanSwapchain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_VulkanLogicalDevice, m_VulkanSwapchain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormatKhr.format;
        m_SwapChainExtent = extent;

        GEOGL_CORE_INFO("Successfully created swapchain and swapchain images.");

    }

    void Context::createImageViews(){

        GEOGL_CORE_INFO("Creating VkImageViews.");
        m_VulkanSwapChainImageViews.resize(m_SwapChainImages.size());

        for(size_t i=0; i < m_SwapChainImages.size(); i++){

            VkImageViewCreateInfo vkImageViewCreateInfo{};
            vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            vkImageViewCreateInfo.image = m_SwapChainImages[i];
            vkImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            vkImageViewCreateInfo.format = m_SwapChainImageFormat;

            /* Color swizzling identity */
            vkImageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            vkImageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            vkImageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            vkImageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            vkImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            vkImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            vkImageViewCreateInfo.subresourceRange.levelCount = 1;
            vkImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            vkImageViewCreateInfo.subresourceRange.layerCount = 1;

            VkResult swapChainImageViewsResult = vkCreateImageView(m_VulkanLogicalDevice, &vkImageViewCreateInfo, nullptr, &m_VulkanSwapChainImageViews[i]);
            GEOGL_CORE_ASSERT_NOSTRIP(swapChainImageViewsResult == VK_SUCCESS, "Unable to create Swap chain Image View #{}, error {}", i, swapChainImageViewsResult);

        }

        GEOGL_CORE_INFO("Sucessfully creaded VkImageViews.");

    }

    void Context::pickPhysicalDevice(){

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

    void Context::createLogicalDevice(){

        GEOGL_CORE_INFO("Creating logical device");

        QueueFamilyIndices indices = findQueueFamilies(m_VulkanPhysicalDevice);
        GEOGL_CORE_ASSERT_NOSTRIP(indices.isComplete(), "Cannot find required queue family indices when creating logical device.");

        {

            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamly.value(), indices.presentFamily.value()};

            float queuePriority = 1.0f;
            for(uint32_t queueFamily:uniqueQueueFamilies){

                /* Info on the queues */

                VkDeviceQueueCreateInfo queueCreateInfo{};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamily;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;

                queueCreateInfos.push_back(queueCreateInfo);
            }


            /* Info on the features we want */
            VkPhysicalDeviceFeatures deviceFeatures{};

            /* Give the parameters to the create info struct, including the
             * queueCreateInfo */
            VkDeviceCreateInfo deviceCreateInfo{};
            deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

            /* queues */
            deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
            deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

            /* features */
            deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

            /* Extensnions */
            deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExtensions.data();

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
        vkGetDeviceQueue(m_VulkanLogicalDevice, indices.presentFamily.value(), 0, &m_VulkanPresentQueue);
        GEOGL_CORE_INFO("Successfully created a logical device and got the queues needed.");

    }

    bool Context::isDeviceSuitable(VkPhysicalDevice device) const{

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

        if(!checkDeviceExtensionSupport(device))
            suitableFlag = false;

        {
            SwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(device);
            if(swapChainSupportDetails.formats.empty() || swapChainSupportDetails.presentModes.empty())
                suitableFlag = false;
        }

        GEOGL_CORE_INFO("Vulkan reports that {} is {}suitable.", deviceProperties.deviceName, (!suitableFlag) ? std::string("not ") : std::string(""));


        return suitableFlag;

    }

    uint64_t Context::rateDeviceSuitablty(VkPhysicalDevice device) const{

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

    bool Context::checkDeviceExtensionSupport(VkPhysicalDevice device) const{

        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(requiredDeviceExtensions.begin(), requiredDeviceExtensions.end());

        for(const auto& extension: availableExtensions){

            requiredExtensions.erase(extension.extensionName);

        }

        return requiredExtensions.empty();

    }

    SwapChainSupportDetails Context::querySwapChainSupport(VkPhysicalDevice device) const{
        SwapChainSupportDetails details{};

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_VulkanSurface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VulkanSurface, &formatCount, nullptr);

        GEOGL_CORE_ASSERT_NOSTRIP(formatCount, "Vulkan reports that no formats are valid for the current vkSurfaceKHR.");
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_VulkanSurface, &formatCount, details.formats.data());
        GEOGL_CORE_INFO("Vulkan reports {} formats are available.", formatCount);

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VulkanSurface, &presentModeCount, nullptr);

        GEOGL_CORE_ASSERT_NOSTRIP(presentModeCount, "Vulkan reports that no present modes are valid for the current vkSurfaceKHR.");
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_VulkanSurface, &presentModeCount, details.presentModes.data());
        GEOGL_CORE_INFO("Vulkan reports {} present modes are available.", presentModeCount);

        return details;

    }

    std::vector<const char*> Context::getRequiredExtensions() const{

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

    QueueFamilyIndices Context::findQueueFamilies(VkPhysicalDevice device) const{

        QueueFamilyIndices indices;

        /* Query vulkan for queue families */
        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        uint32_t i=0;
        for(const auto& queueFamily:queueFamilies){

            /* Graphical support */
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphicsFamly = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_VulkanSurface, &presentSupport);
            if(presentSupport)
                indices.presentFamily = i;

            if(indices.isComplete())
                break;

            ++i;
        }


        return indices;

    }

    bool Context::checkValidationLayerSupport(const std::vector<const char*>& layers) const{

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

    void Context::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) const{

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = Extensions::debugCallback;
        createInfo.pUserData = nullptr;

    }

    void Context::enumerateExtensions() const{
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


    void Context::setupDebugMessenger(){
        if (enableValidationLayers) {

            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            populateDebugMessengerCreateInfo(createInfo);
            VkResult createDebugUtilsResult = Extensions::createDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo, nullptr,
                                                                                             &m_VulkanDebugMessenger);
            GEOGL_CORE_ASSERT_NOSTRIP(createDebugUtilsResult == VK_SUCCESS,
                                 "Unable to create Debug Messenger. Error #{}", createDebugUtilsResult);

        }
    }

}