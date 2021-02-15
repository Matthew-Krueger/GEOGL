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


#ifndef GEOGL_INPUTCODESCONVERTER_HPP
#define GEOGL_INPUTCODESCONVERTER_HPP

#include "InputCodes.hpp"

namespace GEOGL{
    /**
     * \brief Serves as a wrapper to input codes, converting to other types as may be necessary in the future.
     */
    class GEOGL_API_HIDDEN InputCodesConverter{

    public:
        static inline int getNativeKeyCode(KeyCode key) { return s_Instance->getNativeKeyCodeImpl(key); };
        static inline int getNativeMouseCode(MouseCode button) { return s_Instance->getNativeMouseCodeImpl(button); };
        static inline KeyCode getGEOGLKeyCode(int nativeKeyCode) { return s_Instance->getGEOGLKeyCodeImpl(nativeKeyCode); };
        static inline MouseCode getGEOGLMouseCode(int nativeMouseCode) { return s_Instance->getGEOGLMouseCodeImpl(nativeMouseCode); };

        inline static void init(InputCodesConverter* input){s_Instance = input; };

    protected:
        virtual int getNativeKeyCodeImpl(KeyCode key) = 0;
        virtual int getNativeMouseCodeImpl(MouseCode button) = 0;
        virtual KeyCode getGEOGLKeyCodeImpl(int nativeKeyCode) = 0;
        virtual MouseCode getGEOGLMouseCodeImpl(int nativeMouseCode) = 0;

    private:
        static InputCodesConverter* s_Instance;

    };
}

#endif //NODIFY_SCREENWRITER_INPUTCODESCONVERTER_HPP
