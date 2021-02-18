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

#ifndef GEOGL_VULKANINPUT_HPP
#define GEOGL_VULKANINPUT_HPP

#include "../../../IO/Input.hpp"

namespace GEOGL::Platform::Vulkan{

    class GEOGL_API_HIDDEN Input : public GEOGL::Input{

    protected:
        bool isKeyPressedImpl(KeyCode keycode) override;

        bool isMouseButtonPressedImpl(MouseCode button) override;
        bool getMouseXImpl() override;
        bool getMouseYImpl() override;
        glm::vec2 getMousePositionImpl() override;

    };

}

#endif //NODIFY_SCREENWRITER_VULKANINPUT_HPP
