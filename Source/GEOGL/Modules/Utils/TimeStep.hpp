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


#ifndef GEOGL_TIMESTEP_HPP
#define GEOGL_TIMESTEP_HPP

namespace GEOGL{

    /**
     * \brief Helper for determining TimeStep
     */
    class GEOGL_API TimeStep{
    public:
        inline TimeStep(float time = 0.0f) : m_Time(time) {};

        /**
         * \brief Allows implicit casting of this class to a float. This is always seconds
         * @return The casted time in seconds
         */
        inline operator float() const { return m_Time; };

        /**
         * \brief Gets the delta time of the last frame in seconds
         * @return the time of the last frame in seconds
         */
        inline float getSeconds() const { return m_Time; };

        /**
         * \brief The time of the last frame in milliseconds
         * @return
         */
        inline float getMilliseconds() const { return m_Time * 1000.0f; };

    private:
        float m_Time;
    };

}

#endif //GEOGL_TIMESTEP_HPP
