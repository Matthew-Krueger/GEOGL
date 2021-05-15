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

#ifndef GEOGL_GLFWWINDOW_HPP
#define GEOGL_GLFWWINDOW_HPP

#include "../../OpenGL/Rendering/OpenGLGraphicsContext.hpp"
#include "../../../GEOGL/IO/Window.hpp"

/**
 * Forward declaration of GLFWwindow, as we need not know how it works.
 */
struct GLFWwindow;

namespace GEOGL::Platform::GLFW{

    /**
     * \brief Represents an actual OpenGL Window, based on GLFW.
     *
     * This is a pure representation of a window rendering with OpenGL, and contains
     * the OpenGL code required to load the OpenGL extensions from the graphics
     * driver, open the window, and update the frame.
     */
    class GEOGL_API Window : public GEOGL::Window{
    public:
        explicit Window(const WindowProps& props);
        ~Window() override;

        void onUpdate() override;

        [[nodiscard]] inline unsigned int getWidth() const override { return m_Data.width; }
        [[nodiscard]] inline unsigned int getHeight() const override { return m_Data.height; }

        // Window attributes
        /**
         * Sets the event callback for GLFW to use when events are given from the
         * driver.
         * @param callback The callback function to set.
         */
        inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

        /**
         * Sets whether or not VSync is enabled
         * @param enabled Whether or not vsync should be enabled.
         */
        void setVSync(bool enabled) override;

        void setWindowIcon(const std::string& windowIcon) override;

        /**
         * Asks if VSync is enabled
         * @return Whether or not VSync is enabled.
         */
        [[nodiscard]] inline bool isVSync() const override { return m_Data.vSync; };

        /**
         * \brief Gets the pointer to the GLFWwindow
         * @return the GLFWwindow, cast to a void pointer
         */
        [[nodiscard]] inline void* getNativeWindow() const override { return (void*) m_Window; };


        /**
         * \brief Gets that this window is an OpenGL Desktop window
         * @return An GLFW_DESKTOP flag
         */
        inline RendererAPI::WindowingAPIEnum type() override { return RendererAPI::WINDOWING_GLFW_DESKTOP; };

        float getCurrentPlatformTime();

    private:
        /**
         * \brief Sets up the event callbacks for GLFW to be dispatched.
         *
         * \note Calling this function requires that m_Data.EventCallbackFn is set at the time
         * of the callback being called.
         */
        void setUpEventCallbacks();

    private:
        /**
         * \brief Holds the GLFWwindow pointer
         */
        GLFWwindow* m_Window;

        /**
         * \brief Represents the window data that needs to be held as the user pointer
         * in GLFW, used for event callbacks.
         */
        struct WindowData
        {

            /**
             * \brief The Title of the window
             */
            std::string title;

            /**
             * \brief The width and height of the window
             */
            unsigned int width, height;

            /**
             * \brief Whether or not the window enabled vSync.
             */
            bool vSync;

            /**
             * \brief The callback function that is called when an event happens
             */
            EventCallbackFn EventCallback;

            GEOGL::GraphicsContext* graphicsContext;
        };

        /**
         * \brief
         */
        WindowData m_Data;

        GEOGL::GraphicsContext* m_GraphicsContext;

    };

}

#endif //GEOGL_OPENGLWINDOW_HPP
