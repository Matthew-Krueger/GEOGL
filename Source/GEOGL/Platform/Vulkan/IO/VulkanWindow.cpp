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


#include "VulkanWindow.hpp"
#include "../../../IO/Events/ApplicationEvent.hpp"
#include "../../../IO/Events/MouseEvent.hpp"
#include "../../../IO/Events/KeyEvent.hpp"
#include "VulkanInput.hpp"
#include "VulkanKeyCodes.hpp"
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include "../VulkanExtensions/VulkanExtensions.hpp"

namespace GEOGL {

    static uint8_t currentWindows = 0;
    static bool s_GLFWInitialized = false;
    static bool s_GLADInitialized = false;

#ifndef NDEBUG
    bool enableValidationLayers = true;
#else
    bool enableValidationLayers = false;
#endif


    static void glfwErrorCallbackVulkan(int errorCode, const char * errorText){
        GEOGL_CORE_CRITICAL_NOSTRIP("GLFW Error code {}, error text: {}", errorCode, errorText);
    }

    VulkanWindow::VulkanWindow(const WindowProps& props){
        init(props);
    }

    VulkanWindow::~VulkanWindow(){
        shutdown();
    }

    void VulkanWindow::init(const WindowProps& props){

        /* Initialize Input for VulkanWindow */
        Input::init(new VulkanInput());

        /* Initialize Input Polling */
        InputCodesConverter::init(new VulkanKeyCodes());

        /* Set the data of the window */
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;
        GEOGL_CORE_INFO("Creating window {0} ({1}, {2}) using Vulkan.", props.title, props.width, props.height);

        /* Check if GLFW has been initialized, if not, load GLFW */
        if (!s_GLFWInitialized){
            GEOGL_CORE_INFO("Initializing GLFW");
            int success = glfwInit();
            GEOGL_CORE_ASSERT_NOSTRIP(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(&glfwErrorCallbackVulkan);

            s_GLFWInitialized = true;
        }

        /* Create the window */
        {
            GEOGL_CORE_INFO("Creating GLFWWindow with NO context.");
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            m_Window = glfwCreateWindow((int) props.width, (int) props.height, m_Data.title.c_str(), nullptr, nullptr);
            glfwSetWindowUserPointer(m_Window, &m_Data);
            ++currentWindows;
        }

        const std::vector<const char*> validationLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        /* Check validation layer support */
        {
            if(enableValidationLayers && !checkValidationLayerSupport(validationLayers)){
                GEOGL_ASSERT(false, "Validation layers requested, but not supported.");
            }
        }

        /* Create Vulkan Instance */
        {
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = m_Data.title.c_str();
            appInfo.applicationVersion = VK_MAKE_VERSION(props.versionMajor, props.versionMinor, props.versionPatch);
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
        }

        /* Enumerate Extensions */
#if (!defined(NDEBUG) && GEOGL_VULKAN_VERBOSE)
        {
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
#endif

        /* Set up callbacks for GLFW window events */
        {
            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
                auto *data = (WindowData *) glfwGetWindowUserPointer(window);

                data->width = width;
                data->height = height;

                /* Set the viewport before we get further. Better to do now then later */
                //glViewport(0,0,width,height);

                WindowResizeEvent event(width, height);
                data->EventCallback(event);
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
                auto *data = (WindowData *) glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data->EventCallback(event);
            });

            glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
                auto *data = (WindowData *) glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        KeyPressedEvent event(InputCodesConverter::getGEOGLKeyCode(key), 0);
                        data->EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        KeyReleasedEvent event((InputCodesConverter::getGEOGLKeyCode(key)));
                        data->EventCallback(event);
                        break;
                    }
                    case GLFW_REPEAT: {
                        KeyPressedEvent event(InputCodesConverter::getGEOGLKeyCode(key), 1);
                        data->EventCallback(event);
                        break;
                    }
                    default:
                        GEOGL_CORE_CRITICAL_NOSTRIP("Unable to determine key action.");
                        break;
                }
            });

            glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int character) {

                auto data = (WindowData *) glfwGetWindowUserPointer(window);

                KeyTypedEvent event(character);
                data->EventCallback(event);

            });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
                auto data = (WindowData *) glfwGetWindowUserPointer(window);
                switch (action) {
                    case GLFW_PRESS: {
                        MouseButtonPressedEvent event((InputCodesConverter::getGEOGLMouseCode(button)));
                        data->EventCallback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        MouseButtonReleasedEvent event((InputCodesConverter::getGEOGLMouseCode(button)));
                        data->EventCallback(event);
                        break;
                    }
                    default:
                        GEOGL_CORE_CRITICAL_NOSTRIP("Unable to determine mouse action.");
                        break;
                }
            });

            glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {

                auto data = (WindowData *) glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float) xOffset, (float) yOffset);
                data->EventCallback(event);

            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
                auto data = (WindowData *) glfwGetWindowUserPointer(window);

                MouseMovedEvent event((float) xPos, (float) yPos);
                data->EventCallback(event);

            });
        }

        /* set up Vulkan debug messenger */
        if(enableValidationLayers){

            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            populateDebugMessengerCreateInfo(createInfo);
            VkResult createDebugUtilsResult = createDebugUtilsMessengerEXT(m_VulkanInstance, &createInfo,nullptr,&m_VulkanDebugMessenger);
            GEOGL_ASSERT_NOSTRIP(createDebugUtilsResult == VK_SUCCESS, "Unable to create Debug Messenger. Error #{}", createDebugUtilsResult);

        }

    }

    void VulkanWindow::shutdown(){
        --currentWindows;

        /* Clean up vulkan */
        {
            destroyDebugUtilsMessengerEXT(m_VulkanInstance, m_VulkanDebugMessenger, nullptr);

            /* Destroy Instance */
            vkDestroyInstance(m_VulkanInstance, nullptr);
        }

        glfwDestroyWindow(m_Window);
        GEOGL_CORE_INFO("Quit a window. Noting the current window count is {}.", currentWindows);

        /* Check if there are no more windows. If so, terminate GLFW */
        if(currentWindows == 0){
            GEOGL_CORE_INFO("Since current windows is {}, terminating GLFW", currentWindows);
            glfwTerminate();
            s_GLFWInitialized = false;
            s_GLADInitialized = false;
        }

    }

    void VulkanWindow::onUpdate(){
        glfwPollEvents();
    }

    void VulkanWindow::setVSync(bool enabled){
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.vSync = enabled;
    }

    bool VulkanWindow::isVSync() const{
        return m_Data.vSync;
    }

    /*void* OpenGLWindow::getNativeWindow() const{

        return (void*) m_Window;

    }*/

    enum WindowAPIType VulkanWindow::type(){

        return WindowAPIType::WINDOW_OPENGL_DESKTOP;

    }

    void VulkanWindow::clearColor() {

    }

    void VulkanWindow::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;

    }

    bool VulkanWindow::checkValidationLayerSupport(const std::vector<const char*>& layers) const{

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

    std::vector<const char *> VulkanWindow::getRequiredExtensions() {

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

}