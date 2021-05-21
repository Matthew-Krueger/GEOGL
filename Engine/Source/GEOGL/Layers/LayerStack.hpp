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


#ifndef GEOGL_LAYERSTACK_HPP
#define GEOGL_LAYERSTACK_HPP

#include "Layer.hpp"

namespace GEOGL {

    class GEOGL_API LayerStack{
    public:
        /**
         * \brief Creates a new LayerStack
         */
        LayerStack();

        /**
         * \brief Destroys a LayerStack
         */
        ~LayerStack();

        /**
         * \brief Pushes a Layer to the LayerStack
         * \note This transfers the ownership of the layer to the LayerStack, until the time at which it is deleted on
         * destruction of the LayerStack, or popped explicitly. After that time, the object will no longer be owned by
         * the LayerStack
         * @param layer The layer to push
         */
        void pushLayer(Layer* layer);

        /**
         * \brief Pushes a Overlay to the LayerStack
         * \note This transfers the ownership of the overlay to the LayerStack, until the time at which it is deleted on
         * destruction of the LayerStack, or popped explicitly. After that time, the object will no longer be owned by
         * the LayerStack
         * @param overlay The layer to push. (Overlay - a layer that should be inserted on the end of the stack)
         */
        void pushOverlay(Layer* overlay);

        /**
         * Pops a layer from the LayerStack
         * \note This transfers the ownership back to the callee, as the exact ptr to this layer must be known by the
         * caller. Once this pop happens, the layer will no longer be managed by the LayerStack. You must then
         * explicitly delete the Layer.
         * @param layer The layer to pop.
         * @returns If the layer was popped
         */
        bool popLayer(Layer* layer);

        /**
         * Pops a layer from the LayerStack
         * \note This transfers the ownership back to the callee, as the exact ptr to this layer must be known by the
         * caller. Once this pop happens, the layer will no longer be managed by the LayerStack. You must then
         * explicitly delete the Layer.
         * @param layer The layer to pop. (Overlay - a layer that should be inserted on the end of the stack)
         * @return if the overlay was popped
         */
        bool popOverlay(Layer* overlay);

        /**
         * The beginning iterator
         * @return The iterator
         */
        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }

        /**
         * The end iterator
         * @return the iterator
         */
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    private:
        std::vector<Layer*> m_Layers;
        unsigned int m_LayerInsertIndex;
    };

}

#endif //GEOGL_LAYERSTACK_HPP
