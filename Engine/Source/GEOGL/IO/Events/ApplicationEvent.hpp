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

// Based off of Hazel by The Cherno

#ifndef GEOGL_APPLICATIONEVENT_HPP
#define GEOGL_APPLICATIONEVENT_HPP

#include "Event.hpp"

namespace GEOGL{

    /**
     * Represents a window being resized
     */
    class WindowResizeEvent : public Event
    {
    public:

        WindowResizeEvent(unsigned int width, unsigned int height, unsigned int previousWidth, unsigned int previousHeight)
                : m_Width(width), m_Height(height), m_PreviousWidth(width), m_PreviousHeight(height){}

        /**
         * Gets the width the window was resized to
         * @return The width
         */
        unsigned int getWidth() const { return m_Width; }

        /**
         * Gets the height the window was resized to
         * @return The height
         */
        unsigned int getHeight() const { return m_Height; }

        unsigned int getPreviousWidth() const { return m_PreviousWidth; };

        unsigned int getPreviousHeight() const { return m_PreviousHeight; };

        /**
         * Outputs a string describing the event
         * @return The string describing the event
         */
        std::string toString() const override{
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        unsigned int m_Width, m_Height, m_PreviousWidth, m_PreviousHeight;
    };

    /**
     * Represents a close window request
     */
    class WindowCloseEvent : public Event{
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * Represnets a tick
     */
    class AppTickEvent : public Event{
    public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * Represents when the app is updated
     */
    class AppUpdateEvent : public Event{
    public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    /**
     * Represents when the app is rendered
     */
    class AppRenderEvent : public Event{
    public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}

#endif //GEOGL_APPLICATIONEVENT_HPP
