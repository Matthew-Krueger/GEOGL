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

#ifndef GEOGL_PARTICLESYSTEM_HPP
#define GEOGL_PARTICLESYSTEM_HPP

#define GEOGL_SWIZZLE
#include <GEOGL/Core.hpp>
#include <random>


namespace SandboxApp{


    class Random{
    public:
        static void Init(){
            s_RandomEngine.seed(std::random_device()());
            s_Distribution = std::uniform_real_distribution<>(0.0f,1.0f);
        }

        static float Float(){
            //auto limit = (float)0xFFFFFFFF;
            return (float)s_Distribution(s_RandomEngine);//(float)s_Distribution(s_RandomEngine) / limit;
        }

    private:
        static std::mt19937 s_RandomEngine;
        static std::uniform_real_distribution<> s_Distribution;
    };


    struct ParticleProperties{
        glm::vec2 position;
        glm::vec2 velocity, velocityVariation;
        glm::vec4 colorBegin, colorEnd;
        float sizeBegin, sizeEnd, sizeVariation;
        float lifeTime = 1.0f;
    };

    class ParticleSystem{
    public:
        ParticleSystem(uint32_t maxParticles);

        void onUpdate(GEOGL::TimeStep ts);
        void onRender(const GEOGL::OrthographicCamera& camera);

        void emit(const ParticleProperties& particleProperties);
    private:
        struct Particle{
            glm::vec2 position;
            glm::vec2 velocity;
            glm::vec4 colorBegin, colorEnd;
            float rotation = 0.0f;
            float sizeBegin, sizeEnd;

            float lifeTime = 1.0f;
            float lifeRemaining = 0.0f;

            bool active = false;
        };
        std::vector<Particle> m_ParticlePool;
        uint32_t m_PoolIndex;

    };

}

#endif //GEOGL_PARTICLESYSTEM_HPP
