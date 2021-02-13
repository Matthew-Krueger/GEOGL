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


#ifndef NODIFY_SCREENWRITER_IMGUILAYER_HPP
#define NODIFY_SCREENWRITER_IMGUILAYER_HPP

#include "../Events/Event.hpp"
#include "../Rendering/Layer.hpp"

#include "../Events/KeyEvent.hpp"
#include "../Events/MouseEvent.hpp"
#include "../Events/ApplicationEvent.hpp"


namespace GEOGL{

    class GEOGL_API ImGuiLayer : public Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event& event) override;

    private: // functions
        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& event);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
        bool onMouseMovedEvent(MouseMovedEvent& event);
        bool onMouseScrolledEvent(MouseScrolledEvent& event);
        bool onKeyPressedEvent(KeyPressedEvent& event);
        bool onKeyReleasedEvent(KeyReleasedEvent& event);
        bool onKeyTypedEvent(KeyTypedEvent& event);
        bool onWindowResizedEvent(WindowResizeEvent& event);

    private:
        float m_Time = 0;

    };

}

#endif //NODIFY_SCREENWRITER_IMGUILAYER_HPP
