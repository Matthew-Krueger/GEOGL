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


#include "LayerStack.hpp"

namespace GEOGL {

    LayerStack::LayerStack(){
        m_LayerInsertIndex = 0;
    }

    LayerStack::~LayerStack(){
        for (Layer* layer : m_Layers) {
            layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer){
        GEOGL_CORE_INFO("Pushing layer {} to layerstack.", layer->getName());
        m_Layers.emplace(m_Layers.begin()+m_LayerInsertIndex, layer);
        ++m_LayerInsertIndex;
    }

    void LayerStack::pushOverlay(Layer* overlay){
        GEOGL_CORE_INFO("Pushing overlay {} to layerstack.", overlay->getName());
        m_Layers.emplace_back(overlay);
    }

    bool LayerStack::popLayer(Layer* layer){
        auto it = std::find(m_Layers.begin(), m_Layers.begin()+m_LayerInsertIndex, layer);
        if (it != m_Layers.end())
        {
            m_Layers.erase(it);
            --m_LayerInsertIndex;
            return true;
        }

        return false;

    }

    bool LayerStack::popOverlay(Layer* overlay){
        auto it = std::find(m_Layers.begin()+m_LayerInsertIndex, m_Layers.end(), overlay);
        if (it != m_Layers.end()) {
            m_Layers.erase(it);
            return true;
        }
        return false;
    }

}