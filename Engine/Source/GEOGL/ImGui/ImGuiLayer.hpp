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


#ifndef GEOGL_IMGUILAYER_HPP
#define GEOGL_IMGUILAYER_HPP

#include "../IO/Events/Event.hpp"
#include "../Layers/Layer.hpp"

#include "../IO/Events/KeyEvent.hpp"
#include "../IO/Events/MouseEvent.hpp"
#include "../IO/Events/ApplicationEvent.hpp"
#include <ImGui/imgui.h>



namespace GEOGL{


    class GEOGL_API ImGuiLayer : public Layer{
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;

        void begin();
        void end();

        static ImGuiContext* getImGuiContext();

    private:
        float m_Time = 0;

    };

}

#endif //GEOGL_IMGUILAYER_HPP
