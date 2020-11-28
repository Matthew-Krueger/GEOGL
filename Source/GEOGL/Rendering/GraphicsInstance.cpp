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

#include "GraphicsInstance.hpp"
#include "../Utils/Callbacks.hpp"

namespace GEOGL{


    GraphicsInstance::GraphicsInstance(int width, int height, const std::string& windowName) {


        /* Init glfw and crash if unable */
        if(!glfwInit()){
            glfwTerminate();
            GEOGL_CORE_CRITICAL_NOSTRIP("Failed to start GLFW");
            exit(EXIT_FAILURE);
        }

        /* Do window hints for GLFW */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        if(!window){
            glfwTerminate();
            GEOGL_CORE_CRITICAL("Failed to create a GL Window context.");
            exit(EXIT_FAILURE);
        }


        glfwMakeContextCurrent(window);

        /* Init GLAD */
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            GEOGL_CORE_CRITICAL_NOSTRIP("Failed to load OpenGL with GLAD: Glad GLL Loader could not get address of function pointers.");
        }

        /* Set glViewport to what we were given */
        glViewport(0, 0, width, height);

        setSizeCallback(windowDefaultSizeCallback);
        setDebugCallback(windowDefaultDebugCallback);

        GEOGL_CORE_INFO_NOSTRIP("OpenGL Version {}", (const char *)glGetString(GL_VERSION));

    }



    GraphicsInstance::~GraphicsInstance() {

        glfwDestroyWindow(window);
        glfwTerminate();

    }

    void GraphicsInstance::setSizeCallback(void (*windowResizeCallback)(GLFWwindow *, int, int)) {

        glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    }

    void GraphicsInstance::setDebugCallback(void (*errorCallback)(GLenum, GLenum, GLuint, GLenum, GLsizei, const char *, const void *)) { // NOLINT(readability-convert-member-functions-to-static)

        GEOGL_CORE_INFO("Setting glDebugMessageCallback");

#if defined(NDEBUG)
        glEnable(GL_DEBUG_OUTPUT);
#else
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

        glDebugMessageCallback((GLDEBUGPROC) errorCallback, nullptr);

    }

    std::shared_ptr<GEOGL::GraphicsInstance>
    GraphicsInstance::makeGraphicsInstance(int width, int height,const std::string& windowName) {
        return std::make_shared<GEOGL::GraphicsInstance>(width, height, windowName);
    }


}
