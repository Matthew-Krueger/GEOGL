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


#include "InputCodesConverter.hpp"

namespace GEOGL{

    InputCodesConverter* InputCodesConverter::s_Instance = nullptr;

    int InputCodesConverter::getNativeKeyCode(KeyCode key){
        return s_Instance->getNativeKeyCodeImpl(key);
    }

    int InputCodesConverter::getNativeMouseCode(MouseCode button) {
        return s_Instance->getNativeKeyCodeImpl(button);
    }

    KeyCode InputCodesConverter::getGEOGLKeyCode(int nativeKeyCode) {
        return s_Instance->getGEOGLKeyCodeImpl(nativeKeyCode);
    }

    MouseCode InputCodesConverter::getGEOGLMouseCode(int nativeMouseCode) {
        return s_Instance->getGEOGLMouseCodeImpl(nativeMouseCode);
    }



    //InputCodesConverter* iccInstance = nullptr;

}