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

#ifndef GEOGL_WINDOW_HPP
#define GEOGL_WINDOW_HPP

#include "../IO/Events/Event.hpp"
#include <GEOGL/Utils.hpp>

namespace GEOGL {

    /**
     * Holds the properties of a window when it is created
     */
    struct WindowProps
    {

        /**
         * The title of the window to be created
         */
        std::string title;

        std::string iconPath;

        unsigned int versionMajor;
        unsigned int versionMinor;
        unsigned int versionPatch;

        /**
         * The width of the window to be created
         */
        uint16_t width;

        /**
         * The height of the window to be created
         */
        uint16_t height;

        /**
         * Constructs a WindowProps to be passed to the \link Window.create \endlink function.
         * @param appName The title of the window to create (Default: GEOGL Engine")
         * @param width The width of the window to create (Default: 1280 px)
         * @param height The height of the window to create (Default: 720 px)
         * @param appVersionMajor The major version of the app
         * @param appVersionMinor The minor version of the app
         * @param appVersionPatch The patch version of the app
         */
        explicit WindowProps(std::string appName = "GEOGL Engine",
                    uint16_t width = 1280,
                    uint16_t height = 720,
                    std::string imagePath = "",
                    unsigned int appVersionMajor = 0,
                    unsigned int appVersionMinor =0,
                    unsigned int appVersionPatch =1)
                : title(std::move(appName)),
                iconPath(std::move(imagePath)),
                versionMajor(appVersionMajor),
                versionMinor(appVersionMinor),
                versionPatch(appVersionPatch),
                width(width),
                height(height)
        {
        }
    };

    /**
     * \brief Interface representing a desktop system based Window
     * Represents a window. The window is not actually represented here, but instead
     * in the \link Window \endlink class. This is an abstract interface to
     * represent any window of any system, and the things universally required.
     *
     * An event callback must be set in order to handle events, as there is no other
     * interface to handle events for this window type, to keep things cross platform
     * if needed.
     *
     * \note VSync is automatically enabled. To disable, call \link setVSync(false) \endlink
     */
    class GEOGL_API Window{
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(){};
        virtual ~Window() {}
        virtual void onUpdate() = 0;

        /**
         * Gets the current Width of the window.
         * @return The current width of the window
         */
        virtual unsigned int getWidth() const = 0;
        /**
         * Gets the current Height of the window
         * @return The current Height of the window
         */
        virtual unsigned int getHeight() const = 0;

        // Window attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;
        virtual void setWindowIcon(const std::string& windowIcon) = 0;

        /**
         * Gets the native window as a pointer
         * @return The native window pointer void
         */
        virtual void* getNativeWindow() const = 0;

        virtual void clearColor() = 0;

        /**
         * Queries the window type.
         * \note Must be implemented in each platform.
         * @return The type of the window
         */
        virtual enum RenderingAPIType type() = 0;

        /**
         * Creates a window with the selected API.
         * @param api
         * @param props
         * @return
         */
        static Window* create(APIManager& api, const WindowProps& props = WindowProps());
    };

}

#endif //GEOGL_WINDOW_HPP
