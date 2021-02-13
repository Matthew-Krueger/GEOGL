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

/*******************************************************************************
 *                                                                             *
 * This code was based heavily off the Cherno game engine series               *
 *                                                                             *
 *******************************************************************************/


#include "OpenGLWindow.hpp"
#include "../../Events/ApplicationEvent.hpp"
#include "../../Events/MouseEvent.hpp"
#include "../../Events/KeyEvent.hpp"
#include <GLFW/glfw3.h>

namespace GEOGL {

    static uint8_t currentWindows = 0;
    static bool s_GLFWInitialized = false;
    static bool s_GLADInitialized = false;

    void glfwErrorCallback(int errorCode, const char * errorText){
        GEOGL_CORE_CRITICAL_NOSTRIP("GLFW Error code {}, error text: {}", errorCode, errorText);
    }

    Window* Window::create(const WindowProps& props){
        return new OpenGLWindow(props);
    }

    OpenGLWindow::OpenGLWindow(const WindowProps& props){
        init(props);
    }

    OpenGLWindow::~OpenGLWindow(){
        shutdown();
    }

    void OpenGLWindow::init(const WindowProps& props){

        /* Set the data of the window */
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;
        GEOGL_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        /* Check if GLFW has been initialized, if not, load GLFW */
        if (!s_GLFWInitialized){
            // TODO: glfwTerminate on system shutdown
            GEOGL_CORE_INFO("Initializing GLFW");
            int success = glfwInit();
            GEOGL_CORE_ASSERT_NOSTRIP(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(&glfwErrorCallback);

            s_GLFWInitialized = true;
        }


        /* Do window hints for GLFW */
        GEOGL_CORE_INFO("Creating Window with OpenGL Context");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        /* Creating the window */
        m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);

        ++currentWindows;
        GEOGL_CORE_INFO("Successfully created window, noting the current window count is {}.", currentWindows);

        /* Check if higher level openGl funcitons have been loaded */
        if(!s_GLADInitialized) {
            GEOGL_CORE_INFO("Loading higher OpenGL functions with GLAD.");
            int gladStatus = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
            GEOGL_CORE_ASSERT_NOSTRIP(gladStatus, "Failed to load higher OpenGL functions with GLAD.");
        }

        /* Set glViewport to what we were given */
        glViewport(0, 0, props.width, props.height);

        GEOGL_CORE_INFO_NOSTRIP("OpenGL Version: {}.", (const char *)glGetString(GL_VERSION));
        GEOGL_CORE_INFO_NOSTRIP("GLSL Supported version {}.", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

        /* Set up callbacks for GLFW window events */
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height){
            auto* data = (WindowData*) glfwGetWindowUserPointer(window);

            data->width = width;
            data->height = height;

            WindowResizeEvent event(width, height);
            data->EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window){
            auto* data = (WindowData*) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            auto* data = (WindowData*) glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS: {
                    KeyPressedEvent event(key,0);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    KeyReleasedEvent event(key);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    KeyPressedEvent event(key,1);
                    data->EventCallback(event);
                    break;
                }
                default:
                    GEOGL_CORE_CRITICAL_NOSTRIP("Unable to determine key action.");
                    break;
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int character){

            auto data = (WindowData*) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(character);
            data->EventCallback(event);

        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
            auto data = (WindowData*) glfwGetWindowUserPointer(window);
            switch(action){
                case GLFW_PRESS:{
                    MouseButtonPressedEvent event(button);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    MouseButtonReleasedEvent event(button);
                    data->EventCallback(event);
                    break;
                }
                default:
                    GEOGL_CORE_CRITICAL_NOSTRIP("Unable to determine mouse action.");
                    break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset){

            auto data = (WindowData*) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data->EventCallback(event);

        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos){
            auto data = (WindowData*) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data->EventCallback(event);

        });

    }

    void OpenGLWindow::shutdown(){
        --currentWindows;
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

    void OpenGLWindow::onUpdate(){
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void OpenGLWindow::setVSync(bool enabled){
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.vSync = enabled;
    }

    bool OpenGLWindow::isVSync() const{
        return m_Data.vSync;
    }

}