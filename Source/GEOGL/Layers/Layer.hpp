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


#ifndef GEOGL_LAYER_HPP
#define GEOGL_LAYER_HPP

#include "../IO/Events/Event.hpp"

namespace GEOGL {

    class GEOGL_API Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        /**
         * \brief Callback function called when the layer is attached to a LayerStack.
         */
        virtual void onAttach(){}

        /**
         * \brief Callback function called when the layer is detached from a LayerStack.
         * \note This function is guaranteed to be called by any method of detachment. The LayerStack will call this
         * when the layer is detached. The LayerStack will also call this when the LayerStack is destructed if this
         * layer is a current member of the LayerStack. Furthermore, it is important to note that the LayerStack gains
         * ownership of the pointer to this layer while the LayerStack contains it.
         */
        virtual void onDetach(){}

        /**
         * \brief Callback function called every frame for rendering the application.
         *
         * The LayerStack guarantees that onUpdate is called each frame in order of the LayerStack.
         */
        virtual void onUpdate(){}

        /**
         * \brief Callback function for ImGUI Render. Guaranteed to be called each frame.
         *
         * Uses the ImGui Immediate mode renderer to render ImGui graphics. This is called after all onUpdate functions
         * have been called.
         */
        virtual void onImGuiRender(){}

        /**
         * \brief Callback function for Events from the GEOGL Event System.
         *
         * \note This function is NOT guaranteed to be called when an event is dispatched. If an event is dispatched to
         * the Application, the event is propagated down the LayerStack. From here, if an event is marked as handled,
         * the loop is no longer continued to allow for blocking (i.e. clicking on a button shouldn't shoot a gun)
         *
         * @param event The event to be handled.
         */
        virtual void onEvent(Event& event){}

        /**
         * \brief Gets the name of the current layer. Should not be used for release builds.
         * @return The name of the layer.
         */
        inline const std::string& getName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}

#endif //GEOGL_LAYER_HPP
