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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include "OpenGLGraphicsContext.hpp"

namespace GEOGL::Platform::OpenGL{

    static bool s_GLADInitialized = false;

    /*
     * Error Callback
     */

    void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam )
    {
        GEOGL_RENDERER_PROFILE_FUNCTION();


        std::string header = GEOGL_FORMAT(      "Open GL {}, ID {}:", glGetString(severity), id);
        std::string errorType = GEOGL_FORMAT(   "   Type: {}", glGetString(type));
        std::string errorSource = GEOGL_FORMAT( "   Source: {}", glGetString(source));
        std::string errorMessage = GEOGL_FORMAT("   Message: {}", message);

        switch(severity){
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                GEOGL_CORE_INFO(header);
                GEOGL_CORE_INFO(errorType);
                GEOGL_CORE_INFO(errorSource);
                GEOGL_CORE_INFO(errorMessage);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                GEOGL_CORE_INFO_NOSTRIP(header);
                GEOGL_CORE_INFO_NOSTRIP(errorType);
                GEOGL_CORE_INFO_NOSTRIP(errorSource);
                GEOGL_CORE_INFO_NOSTRIP(errorMessage);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                GEOGL_CORE_WARN_NOSTRIP(header);
                GEOGL_CORE_WARN_NOSTRIP(errorType);
                GEOGL_CORE_WARN_NOSTRIP(errorSource);
                GEOGL_CORE_WARN_NOSTRIP(errorMessage);
                break;
            case GL_DEBUG_SEVERITY_HIGH:
                GEOGL_CORE_CRITICAL_NOSTRIP(header);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorType);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorSource);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorMessage);
                break;
            default:
                GEOGL_CORE_CRITICAL_NOSTRIP(header);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorType);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorSource);
                GEOGL_CORE_CRITICAL_NOSTRIP(errorMessage);
        }
    }


    /*
     * Graphics context
     */
    GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle){
        GEOGL_PROFILE_FUNCTION();

        GEOGL_CORE_INFO("Loading OpenGL Graphics Context using GLAD");
        GEOGL_CORE_ASSERT(windowHandle, "Window Handle cannot be NULL.");

        {
            GEOGL_PROFILE_SCOPE("Making GLFW Context Current");
            glfwMakeContextCurrent(m_WindowHandle);
        }

        /* Check if higher level openGl funcitons have been loaded */
        if(!s_GLADInitialized) {
            GEOGL_PROFILE_SCOPE("Loading GLAD and enabling debugmessenger.");
            int gladStatus = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
            GEOGL_CORE_ASSERT_NOSTRIP(gladStatus, "Failed to load higher OpenGL functions with GLAD.");

            /* now, set the debug callback */
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(MessageCallback,nullptr);
            s_GLADInitialized = true;
        }

    }

    GraphicsContext::~GraphicsContext() {

        s_GLADInitialized = false;

    }

    void GraphicsContext::setViewport(const glm::ivec2& topLeftCorner, const glm::ivec2& dimensions){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glViewport(topLeftCorner.x, topLeftCorner.y, dimensions.x, dimensions.y);
    }

    void GraphicsContext::clearColor(){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glClearColor(.1f,.1f,.1f,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }

    void GraphicsContext::setVSync(bool* vSyncStatus){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glfwSwapInterval(*vSyncStatus);

    }

    void GraphicsContext::swapBuffers() {
        GEOGL_RENDERER_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);

    }

    void GraphicsContext::deInitGlad(){
        GEOGL_RENDERER_PROFILE_FUNCTION();

        s_GLADInitialized = false;

    }
}
