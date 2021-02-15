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

#ifndef GEOGL_KEYEVENT_HPP
#define GEOGL_KEYEVENT_HPP

//#include "../../Utils/KeyCodes.hpp"
#include "../../Utils/InputCodes.hpp"

namespace GEOGL{

    /**
     * RepresentsA key event
     */
    class KeyEvent : public Event{
    public:
        KeyCode getKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(const KeyCode keycode)
                : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    /**
     * Represents when a key is pressed
     */
    class KeyPressedEvent : public KeyEvent{
    public:
        KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
                : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        uint16_t GetRepeatCount() const { return m_RepeatCount; }

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        uint16_t m_RepeatCount;
    };

    /**
     * Represents when a key is released
     */
    class KeyReleasedEvent : public KeyEvent{
    public:
        KeyReleasedEvent(const KeyCode keycode)
                : KeyEvent(keycode) {}

        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    /**
     * Represents when a key is depressed
     */
    class KeyTypedEvent : public KeyEvent{
    public:
        KeyTypedEvent(const KeyCode keycode)
                : KeyEvent(keycode) {}

        std::string toString() const override{
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };

}

#endif //GEOGL_KEYEVENT_HPP
