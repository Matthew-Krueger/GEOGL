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


#ifndef GEOGL_VULKANWINDOW_HPP
#define GEOGL_VULKANWINDOW_HPP

#include "../../../IO/Window.hpp"
#include <vulkan/vulkan.h>

/**
 * Forward declaration of GLFWwindow, as we need not know how it works.
 */
struct GLFWwindow;

namespace GEOGL {

    /**
     * \brief Represents an actual OpenGL Window, based on GLFW.
     *
     * This is a pure representation of a window rendering with OpenGL, and contains
     * the OpenGL code required to load the OpenGL extensions from the graphics
     * driver, open the window, and update the frame.
     */
    class GEOGL_API_HIDDEN VulkanWindow : public Window {
    public:
        explicit VulkanWindow(const WindowProps &props);
        ~VulkanWindow() override;

        void onUpdate() override;
        inline unsigned int getWidth() const override { return m_Data.width; }
        inline unsigned int getHeight() const override { return m_Data.height; }

        // Window attributes
        /**
         * Sets the event callback for GLFW to use when events are given from the
         * driver.
         * @param callback The callback function to set.
         */
        inline void setEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; }

        /**
         * Sets whether or not VSync is enabled
         * @param enabled Whether or not vsync should be enabled.
         */
        void setVSync(bool enabled) override;

        /**
         * Asks if VSync is enabled
         * @return Whether or not VSync is enabled.
         */
        bool isVSync() const override;

        inline void *getNativeWindow() const override { return m_Window; };

        void clearColor() override;

        /**
         * Returns that this window is an OpenGL Desktop window
         * @return An WINDOW_OPENGL_DESKTOP flag
         */
        enum WindowAPIType type() override;

    private:
        void init(const WindowProps &props);
        void shutdown();

        bool checkValidationLayerSupport(const std::vector<const char*>& layers) const;
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();

    private:
        /**
         * Holds the window ptr.
         */
        GLFWwindow *m_Window;

        /**
         * Represents the data held by the window pointer
         */
        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool vSync;

            EventCallbackFn EventCallback;
        };

        /**
         * The data held by the window pointer
         */
        WindowData m_Data;

    private:
        VkInstance m_VulkanInstance;
        VkDebugUtilsMessengerEXT m_VulkanDebugMessenger;

    };

}
#endif //NODIFY_SCREENWRITER_VULKANWINDOW_HPP
