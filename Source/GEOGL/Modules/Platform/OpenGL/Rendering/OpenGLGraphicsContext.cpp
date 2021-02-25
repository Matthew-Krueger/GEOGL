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
                     const void* userParam ){

        GEOGL_CORE_WARN_NOSTRIP("OpenGL {}:", glGetString(severity));
        GEOGL_CORE_WARN_NOSTRIP("   Type: {}", glGetString(type));
        GEOGL_CORE_WARN_NOSTRIP("   Source: {}", glGetString(source));
        GEOGL_CORE_WARN_NOSTRIP("   Message: {}", message);

    }

    /*
     * Graphics context
     */
    GraphicsContext::GraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle){

        GEOGL_CORE_ASSERT(windowHandle, "Window Handle cannot be NULL.");

        glfwMakeContextCurrent(m_WindowHandle);

        /* Check if higher level openGl funcitons have been loaded */
        if(!s_GLADInitialized) {
            GEOGL_CORE_INFO("Loading higher OpenGL functions with GLAD.");
            int gladStatus = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
            GEOGL_CORE_ASSERT_NOSTRIP(gladStatus, "Failed to load higher OpenGL functions with GLAD.");

            /* now, set the debug callback */
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(MessageCallback,0);
        }

        GEOGL_CORE_INFO_NOSTRIP("OpenGL Info:");
        GEOGL_CORE_INFO_NOSTRIP("   Vendor: {}", glGetString(GL_VENDOR));
        GEOGL_CORE_INFO_NOSTRIP("   Renderer: {}", glGetString(GL_RENDERER));
        GEOGL_CORE_INFO_NOSTRIP("   OpenGL Version: {}.", (const char *)glGetString(GL_VERSION));
        GEOGL_CORE_INFO_NOSTRIP("   GLSL Version {}.", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

    }

    GraphicsContext::~GraphicsContext() {

    }

    void GraphicsContext::setViewport(const glm::vec2& topLeftCorner, const glm::vec2& dimensions){
        glViewport(topLeftCorner.x, topLeftCorner.y, dimensions.x, dimensions.y);
    }

    void GraphicsContext::clearColor(){

        glClearColor(.1f,.1f,.1f,1);
        glClear(GL_COLOR_BUFFER_BIT);

    }

    void GraphicsContext::setVSync(bool* vSyncStatus){

        glfwSwapInterval(*vSyncStatus);

    }

    void GraphicsContext::swapBuffers() {



        glfwSwapBuffers(m_WindowHandle);

    }

    void GraphicsContext::deInitGlad(){

        s_GLADInitialized = false;

    }
}
