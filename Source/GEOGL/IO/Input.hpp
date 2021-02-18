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

#ifndef GEOGL_INPUT_HPP
#define GEOGL_INPUT_HPP

#include <GEOGL/Utils.h>

namespace GEOGL{

    class GEOGL_API Input{

    public:
        /**
         * Asks GEOGL if the key specified is pressed or not.
         * @param keycode The key to check
         * @return Whether or not the key is pressed
         */
        inline static bool isKeyPressed(KeyCode keycode){ return s_Instance->isKeyPressedImpl(keycode); };

        inline static bool isMouseButtonPressed(MouseCode button){ return s_Instance->isMouseButtonPressedImpl(button); };
        inline static float getMouseX(){return s_Instance->getMouseXImpl(); };
        inline static float getMouseY(){return s_Instance->getMouseYImpl(); };
        inline static glm::vec2 getMousePosition(){return s_Instance->getMousePositionImpl();};

        inline static void init(Input* input){s_Instance = input; };
    protected:
        virtual bool isKeyPressedImpl(KeyCode keycode)=0;
        virtual bool isMouseButtonPressedImpl(MouseCode button) = 0;
        virtual bool getMouseXImpl() = 0;
        virtual bool getMouseYImpl() = 0;
        virtual glm::vec2 getMousePositionImpl() = 0;

    private:
        static Input* s_Instance;

    };

}

#endif //NODIFY_SCREENWRITER_INPUT_HPP
