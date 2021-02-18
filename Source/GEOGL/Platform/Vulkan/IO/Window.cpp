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


#include "Window.hpp"
#include "../VulkanHandler/Context.hpp"
#include "../../../IO/Events/ApplicationEvent.hpp"
#include "../../../IO/Events/MouseEvent.hpp"
#include "../../../IO/Events/KeyEvent.hpp"
#include "Input.hpp"
#include "KeyCodes.hpp"
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

namespace GEOGL::Platform::Vulkan{

    static uint8_t currentWindows = 0;
    static bool s_GLFWInitialized = false;
    static bool s_GLADInitialized = false;


    static void glfwErrorCallbackVulkan(int errorCode, const char * errorText){
        GEOGL_CORE_CRITICAL_NOSTRIP("GLFW Error code {}, error text: {}", errorCode, errorText);
    }

    Window::Window(const WindowProps& props){
        init(props);
    }

    Window::~Window(){
        shutdown();
    }

    void Window::init(const WindowProps& props){

        /* Initialize Input for Window */
        Input::init(new Input());

        /* Initialize Input Polling */
        InputCodesConverter::init(new KeyCodes());

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


        /* Create Vulkan Instance */
        m_VulkanContext = new Context(m_Data.title.c_str(), props);

        /* Enumerate Extensions */
#if (!defined(NDEBUG) && defined(GEOGL_VULKAN_VERBOSE))
        VulkanHandler::enumerateExtensions();
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



    }

    void Window::shutdown(){
        --currentWindows;

        /* Clean up vulkan */
        delete m_VulkanContext;

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

    void Window::onUpdate(){
        glfwPollEvents();
    }

    void Window::setVSync(bool enabled){

        GEOGL_CORE_ERROR("Cannot set vsync yet on vulkan");

    }

    bool Window::isVSync() const{
        return m_Data.vSync;
    }

    /*void* Window::getNativeWindow() const{

        return (void*) m_Window;

    }*/

    enum WindowAPIType Window::type(){

        return WindowAPIType::WINDOW_VULKAN_DESKTOP;

    }

    void Window::clearColor() {

    }

}