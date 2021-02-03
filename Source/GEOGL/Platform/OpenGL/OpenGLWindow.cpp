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

namespace GEOGL {


    void glfwErrorCallback(int errorCode, const char * errorText){
        GEOGL_CORE_CRITICAL_NOSTRIP("GLFW Error code {}, error text: {}", errorCode, errorText);
    }

    static bool s_GLFWInitialized = false;

    Window* Window::create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        init(props);
    }
#pragma clang diagnostic pop

    WindowsWindow::~WindowsWindow()
    {
        shutdown();
    }

    void WindowsWindow::init(const WindowProps& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        GEOGL_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        if (!s_GLFWInitialized)
        {
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            GEOGL_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(&glfwErrorCallback);

            s_GLFWInitialized = true;
        }


        /* Do window hints for GLFW */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        setVSync(true);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            GEOGL_CORE_CRITICAL_NOSTRIP("Failed to load OpenGL with GLAD");
        }

        /* Set glViewport to what we were given */
        glViewport(0, 0, props.width, props.height);

    }

    void WindowsWindow::shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::onUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::setVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.vSync = enabled;
    }

    bool WindowsWindow::isVSync() const
    {
        return m_Data.vSync;
    }

}