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

#ifndef NODIFY_SCREENWRITER_OPENGLWINDOW_HPP
#define NODIFY_SCREENWRITER_OPENGLWINDOW_HPP

#include "../../Rendering/Window.hpp"

/**
 * Forward declaration of GLFWwindow, as we need not know how it works.
 */
struct GLFWwindow;

namespace GEOGL {

    /**
     * Represents an actual OpenGL Window, based on GLFW.
     */
    class OpenGLWindow : public Window
    {
    public:
        OpenGLWindow(const WindowProps& props);
        virtual ~OpenGLWindow();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return m_Data.width; }
        inline unsigned int getHeight() const override { return m_Data.height; }

        // Window attributes
        inline void setEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void setVSync(bool enabled) override;
        bool isVSync() const override;
    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();
    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string title;
            unsigned int width, height;
            bool vSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}

#endif //NODIFY_SCREENWRITER_OPENGLWINDOW_HPP
