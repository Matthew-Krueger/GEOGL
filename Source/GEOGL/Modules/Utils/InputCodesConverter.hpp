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

    class InputCodesConverter;
    extern InputCodesConverter* s_Instance;

    /**
     * \brief Serves as a wrapper to input codes, converting to other types as may be necessary in the future.
     *
     * \note This is a singleton. Just call InputCodesConverter::function() to convert anywhere in the program. Creating
     * this is handled by the window.
     *
     * \todo Do we want to move this to the Application class as it seems to handle the other singletons?
     */
    class GEOGL_API InputCodesConverter{

    public:

        /**
         * \brief Converts a GEOGL KeyCode to a native keycode.
         * @param key The GEOGL Code
         * @return The Native code
         */
        static int getNativeKeyCode(KeyCode key);
        /**
         * \brief Converts a GEOGL MouseCode to a native mousecode.
         * @param key The GEOGL Code
         * @return The Native code
         */
        static int getNativeMouseCode(MouseCode button);
        /**
         * \brief Converts a native keycode to a GEOGL KeyCode.
         * @param key The Native Code
         * @return The GEOGL code
         */
        static KeyCode getGEOGLKeyCode(int nativeKeyCode);
        /**
         * \brief Converts a native mousecode to a GEOGL MouseCode.
         * @param key The Native Code
         * @return The GEOGL code
         */
        static MouseCode getGEOGLMouseCode(int nativeMouseCode);

        inline static void init(InputCodesConverter* input){ s_Instance = input; };

    protected:
        virtual int getNativeKeyCodeImpl(KeyCode key) = 0;
        virtual int getNativeMouseCodeImpl(MouseCode button) = 0;
        virtual KeyCode getGEOGLKeyCodeImpl(int nativeKeyCode) = 0;
        virtual MouseCode getGEOGLMouseCodeImpl(int nativeMouseCode) = 0;

    private:
        static InputCodesConverter* s_Instance;

    };

}

#endif //GEOGL_INPUTCODESCONVERTER_HPP
