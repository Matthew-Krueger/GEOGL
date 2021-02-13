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

#ifndef GEOGL_EVENT_HPP
#define GEOGL_EVENT_HPP

namespace GEOGL{

#define GEOGL_BIND_EVENT_FN(function) std::bind(&function, this, std::placeholders::_1)


    /**
     * Represents the type of an event
     */
    enum class EventType{
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    /**
     * Represents the category of an event
     */
    enum EventCategory{
        None = 0,
        EventCategoryApplication    = BIT(0),
        EventCategoryInput          = BIT(1),
        EventCategoryKeyboard       = BIT(2),
        EventCategoryMouse          = BIT(3),
        EventCategoryMouseButton    = BIT(4)
    };

    /**
     * Fills the types of events as functions
     */
#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

    /**
     * Represents an event
     */
    class Event{
    public:
        virtual ~Event() = default;

        bool Handled = false;

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const { return getName(); }

        /**
         * If this event is a specific category
         * @param category
         * @return
         */
        bool IsInCategory(EventCategory category){
            return getCategoryFlags() & category;
        }
    };

    /**
     * Dispatches an event.
     * Usage:<br>
     * \code EventDispatcher dispatcher(event);
     * dispatcher.dispatch<MouseMovedEvent>(GEOGL_BIND_FUNCTION(onMouseMovedEvent)); \endcode
     */
    class EventDispatcher
    {
    public:
        /**
         * Creates a dispatcher which is capable of dispatching events using a template
         * parameter as illustrated in
         * @param event
         */
        EventDispatcher(Event& event)
                : m_Event(event){}

        // F will be deduced by the compiler
        /**
         * Dispatches the event to the function func, if it matches
         * the type of event stored in the dispatcher.
         * @tparam EventType The Type of the event you wish to target. Note, this is not
         * the same as the type of event given to dispatcher, this is the actual target type.
         * @tparam FunctionType This template perimeter can be automatically deduced by the compiler.
         * Buy it a beer (but not for Clang. It is not 21 yet).
         * @param functionToBind The function to bind if EventType and the type of the internal event
         * match.
         * @return True if the event was dispatched, false otherwise.
         * \example EventDispatcher dispatcher(event);
         * dispatcher.dispatch<MouseMovedEvent>(GEOGL_BIND_FUNCTION(onMouseMovedEvent));
         */
        template<typename EventType, typename FunctionType>
        bool dispatch(const FunctionType& functionToBind){
            if (m_Event.getEventType() == EventType::getStaticType()){
                m_Event.Handled |= functionToBind(static_cast<EventType&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.toString();
    }

}

#endif //GEOGL_EVENT_HPP
