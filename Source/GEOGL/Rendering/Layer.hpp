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


#ifndef NODIFY_SCREENWRITER_LAYER_HPP
#define NODIFY_SCREENWRITER_LAYER_HPP

#include "../Events/Event.hpp"

namespace GEOGL {

    class GEOGL_API Layer{
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void onAttach(){}
        virtual void onDetach(){}
        virtual void onUpdate() = 0;
        virtual void onEvent(Event& event) {}

        inline const std::string& getName() const { return m_DebugName; }
    protected:
        std::string m_DebugName;
    };

}

#endif //NODIFY_SCREENWRITER_LAYER_HPP
