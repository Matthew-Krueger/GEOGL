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

#ifndef GEOGL_OPENGLKEYCODES_HPP
#define GEOGL_OPENGLKEYCODES_HPP

#include "../../../Utils/InputCodes.hpp"
#include "../../../Utils/InputCodesConverter.hpp"

namespace GEOGL::Platform::GLFW{

    /**
     * \brief A wrapper for GLFW Key Codes.
     *
     * Since GLFW is identical to our keycodes, these just return the cast to int of the code.
     */
    class GEOGL_API_HIDDEN GLFWKeyCodes : public GEOGL::InputCodesConverter{
    private:
        int getNativeKeyCodeImpl(KeyCode key) override;
        int getNativeMouseCodeImpl(MouseCode button) override;
        KeyCode getGEOGLKeyCodeImpl(int nativeKeyCode) override;
        MouseCode getGEOGLMouseCodeImpl(int nativeMouseCode) override;

    };

}

#endif //GEOGL_OPENGLKEYCODES_HPP
