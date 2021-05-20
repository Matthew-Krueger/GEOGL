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


/* GLAD */
#include <glad/glad.h>

/* GLFW */
#include <GLFW/glfw3.h>

#include "GLFWWindow.hpp"
#include "GLFWInput.hpp"
#include "GLFWKeyCodes.hpp"

#include "../../../GEOGL/Rendering/Renderer.hpp"
#include "../../../GEOGL/IO/Events/ApplicationEvent.hpp"
#include "../../../GEOGL/IO/Events/KeyEvent.hpp"
#include "../../../GEOGL/IO/Events/MouseEvent.hpp"

#if GEOGL_BUILD_WITH_OPENGL == 1
#include "../../OpenGL/Rendering/OpenGLGraphicsContext.hpp"
#endif

#if GEOGL_BUILD_WITH_VULKAN == 1
#include "../../Vulkan/Rendering/VulkanGraphicsContext.hpp"
#endif
namespace GEOGL::Platform::GLFW{

    static uint8_t currentWindows = 0;
    static bool s_GLFWInitialized = false;
    static bool s_UsedGLAD = false;

    static void glfwErrorCallbackOpenGL(int errorCode, const char * errorText){
        GEOGL_PROFILE_FUNCTION();

        GEOGL_CORE_CRITICAL_NOSTRIP("GLFW Error code {}, error text: {}", errorCode, errorText);
    }

    Window::Window(const WindowProps& props){
        GEOGL_PROFILE_FUNCTION();

        m_Window = nullptr;

        /* Get all of the rendering api data */
        auto renderingAPI = Renderer::getRendererAPI()->getRenderingAPI();

        /* Initialize Input for Window */
        Input::init(new Input());

        /* Initialize Input Polling */
        InputCodesConverter::init(new GLFWKeyCodes());

        /* Set the data of the window */
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;
        GEOGL_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

        /* Check if GLFW has been initialized, if not, load GLFW */
        if (!s_GLFWInitialized){
            GEOGL_PROFILE_SCOPE("Init GLFW");
            // TODO: glfwTerminate on system shutdown
            GEOGL_CORE_INFO("Initializing GLFW");
            int success = glfwInit();
            GEOGL_CORE_ASSERT_NOSTRIP(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(&glfwErrorCallbackOpenGL);

            s_GLFWInitialized = true;
        }

        /* Guard against incompatible apis */
        if((renderingAPI == RendererAPI::RENDERING_VULKAN_DESKTOP)&&((!glfwVulkanSupported())||(!GEOGL_BUILD_WITH_VULKAN))){
            GEOGL_CORE_CRITICAL("Vulkan selected but not supported.");
            exit(1);
        }
        if(renderingAPI == RendererAPI::RENDERING_OPENGL_DESKTOP)
        if(!(GEOGL_BUILD_WITH_OPENGL)){
            GEOGL_CORE_CRITICAL("OpenGL selected but not supported.");
            exit(1);
        }

        /* Do window hints for GLFW */
        switch(renderingAPI){
            case RendererAPI::RENDERING_OPENGL_DESKTOP:
                {
                    GEOGL_PROFILE_SCOPE("Sending window hints");

                    GEOGL_CORE_INFO("Creating Window with OpenGL Context");
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

                    s_UsedGLAD = true;
                }
                /* Creating the window */
                {
                    GEOGL_PROFILE_SCOPE("Creating GLFW Window");
                    m_Window = glfwCreateWindow((int) props.width, (int) props.height, m_Data.title.c_str(), nullptr,
                                                nullptr);
                }

                /* Create context */
#if GEOGL_BUILD_WITH_OPENGL == 1
                m_GraphicsContext = new GEOGL::Platform::OpenGL::GraphicsContext(m_Window);
#else
                GEOGL_CORE_CRITICAL_NOSTRIP("Requested OpenGL,but OpenGL not supported.");
#endif
                break;
            case RendererAPI::RENDERING_VULKAN_DESKTOP:

                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

                /* Creating the window */
                m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);

#if GEOGL_BUILD_WITH_VULKAN == 1
                m_GraphicsContext = new GEOGL::Platform::Vulkan::GraphicsContext(m_Window, props.title, props.versionMajor, props.versionMinor, props.versionPatch);
#else
                GEOGL_CORE_CRITICAL_NOSTRIP("Requested Vulkan, but Vulkan Not Supported");
#endif
                break;
            default:
                GEOGL_CORE_CRITICAL("Error. No valid Graphics API selected. Please select {} for OpenGL in settings.json.", RendererAPI::RENDERING_VULKAN_DESKTOP);
                exit(1);

        }
        glfwSetWindowUserPointer(m_Window, &m_Data);
        m_Data.graphicsContext = m_GraphicsContext;
        m_Data.m_WindowDimensions = {props.width, props.height};

        /* Set VSync true. Evidently, no virtual member functions from the constructor. */
        {
            bool vsync = true;
            m_GraphicsContext->setVSync(&vsync); // set vsync true
            m_Data.vSync = vsync;
        }

        ++currentWindows;
        GEOGL_CORE_INFO("Successfully created window, noting the current window count is {}.", currentWindows);

        /* Set glViewport to what we were given */
        m_GraphicsContext->setViewport({0,0}, {props.width,props.height});

        /* Set up callbacks for GLFW window events */
        {
            setUpEventCallbacks();
        }

        GEOGL_CORE_ASSERT(m_Window, "Did not successfully create the window");

    }

    Window::~Window(){
        GEOGL_PROFILE_FUNCTION();

        --currentWindows;
        glfwDestroyWindow(m_Window);
        GEOGL_CORE_INFO("Quit a window. Noting the current window count is {}.", currentWindows);

        /* Check if there are no more windows. If so, terminate GLFW */
        if(currentWindows == 0){
            GEOGL_CORE_INFO("Since current windows is {}, terminating GLFW", currentWindows);
            glfwTerminate();
            s_GLFWInitialized = false;
            if(s_UsedGLAD)
                ((GEOGL::Platform::OpenGL::GraphicsContext*)m_GraphicsContext)->deInitGlad();
        }

        delete m_GraphicsContext;
    }

    void Window::onUpdate(){
        GEOGL_PROFILE_FUNCTION();

        glfwPollEvents();
        m_GraphicsContext->swapBuffers();
    }

    void Window::setVSync(bool enabled){
        GEOGL_PROFILE_FUNCTION();

        m_GraphicsContext->setVSync(&enabled);
        m_Data.vSync = enabled;

    }

    void Window::setWindowIcon(const std::string& windowIcon){
        GEOGL_PROFILE_FUNCTION();

        if(windowIcon.empty())
            return;
        GLFWimage image[1];
        {
            GEOGL_PROFILE_SCOPE("Loading image into memory");
            image->pixels = stbi_load(windowIcon.c_str(), &image->width, &image->height, 0, 4);
        }
        GEOGL_ASSERT(image->pixels, "Could not load image");
        {
            GEOGL_PROFILE_SCOPE("Loading image into window");
            glfwSetWindowIcon(m_Window, 1, image);
        }
        stbi_image_free(image->pixels);

    }

    float Window::getCurrentPlatformTime(){
        return static_cast<float>(glfwGetTime());
    }

    void Window::setUpEventCallbacks(){
        GEOGL_PROFILE_FUNCTION();

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
            GEOGL_PROFILE_FUNCTION();

            auto *data = (WindowData *) glfwGetWindowUserPointer(window);

            data->width = width;
            data->height = height;

            /* send the event, with m_WindowDimensions as old dimensions */
            WindowResizeEvent event(width, height, data->m_WindowDimensions.x, data->m_WindowDimensions.y);
            data->EventCallback(event);

            /* Now, set the old dimensions to this one */
            data->m_WindowDimensions = {width, height};
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
            GEOGL_PROFILE_FUNCTION();

            auto *data = (WindowData *) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            GEOGL_PROFILE_FUNCTION();

            auto *data = (WindowData *) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(InputCodesConverter::getGEOGLKeyCode(key), 0);
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(InputCodesConverter::getGEOGLKeyCode(key));
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
            GEOGL_PROFILE_FUNCTION();

            auto data = (WindowData *) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(character);
            data->EventCallback(event);

        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
            GEOGL_PROFILE_FUNCTION();

            auto data = (WindowData *) glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(InputCodesConverter::getGEOGLMouseCode(button));
                    data->EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(InputCodesConverter::getGEOGLMouseCode(button));
                    data->EventCallback(event);
                    break;
                }
                default:
                    GEOGL_CORE_CRITICAL_NOSTRIP("Unable to determine mouse action.");
                    break;
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
            GEOGL_PROFILE_FUNCTION();

            auto data = (WindowData *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data->EventCallback(event);

        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
            GEOGL_PROFILE_FUNCTION();

            auto data = (WindowData *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPos, (float) yPos);
            data->EventCallback(event);

        });

    }

}