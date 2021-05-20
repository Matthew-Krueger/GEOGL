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

#ifndef GEOGL_TIMER_HPP
#define GEOGL_TIMER_HPP

namespace GEOGL{

    struct ProfileResult{
        const char* name;
        float time;
    };

    template<typename Fn>
    class Timer{
    public:
        Timer(const char * name, Fn&& callback)
                : m_Name(name), m_Stopped(false), m_Callback(callback){

            m_StartTimePoint = std::chrono::high_resolution_clock::now();

        }

        inline ~Timer() { if(!m_Stopped) stop(); };

        void stop(){

            auto endTimePoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            m_Stopped = true;

            m_Callback({m_Name, (float)(end-start)*.001f});

        }

    private:
        const char * m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
        bool m_Stopped;
        Fn m_Callback;

    };

#define PROFILE_SCOPE(name) GEOGL::Timer timer##__func__##__LINE__(name, [&](GEOGL::ProfileResult profileResult){ m_ProfileResults.push_back(profileResult); })

}

#endif //GEOGL_TIMER_HPP
