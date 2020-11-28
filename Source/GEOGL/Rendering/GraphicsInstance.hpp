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

#ifndef GEOGL_GRAPHICSINSTANCE_HPP
#define GEOGL_GRAPHICSINSTANCE_HPP

namespace GEOGL {
    class GEOGL_API GraphicsInstance {
    private:
        GLFWwindow *window;

    public:
        GraphicsInstance(int width, int height, const std::string& windowName);
        ~GraphicsInstance();

        /**
         * \brief Sets a window resize callback.
         * @param window The window to set the callback on
         * @param windowResizeCallback The Callback to set
         */
        void setSizeCallback(void (*windowResizeCallback)(GLFWwindow *window, int width, int height));

        /**
         * \brief Set an error callback for OpenGL
         * @param errorCallback The Error Callback to set
         */
        void setDebugCallback(void (*errorCallback)(GLenum, GLenum, GLuint, GLenum, GLsizei, const char *, const void *));

        /**
         * Gets the ptr to the window
         * @return The GLFWwindow*
         */
        inline GLFWwindow *getWindowPtr(){ return window; }

        /**
         * Asks if the graphics should close.
         * @return If the window should close
         */
        inline bool windowShouldClose(){ return glfwWindowShouldClose(window); }

        /**
         * Updates the window and swaps the swapchain
         */
        inline void updateDisplay(){ glfwSwapBuffers(window); glfwPollEvents(); };
        inline void closeDisplay(){ glfwSetWindowShouldClose(window, true); };

        static std::shared_ptr<GEOGL::GraphicsInstance> makeGraphicsInstance(int width, int height, const std::string& windowName);

    };
}


#endif //GEOGL_GRAPHICSINSTANCE_HPP
