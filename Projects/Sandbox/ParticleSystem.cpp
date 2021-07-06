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


#include "ParticleSystem.hpp"
#include <GEOGL/Renderer.hpp>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace SandboxApp{

    std::mt19937 Random::s_RandomEngine;
    std::uniform_real_distribution<> Random::s_Distribution;

    ParticleSystem::ParticleSystem(uint32_t maxParticles) : m_PoolIndex(maxParticles-1){
        Random::Init();
        m_ParticlePool.resize(maxParticles);
    }

    void ParticleSystem::onUpdate(GEOGL::TimeStep ts){

        for (auto& particle : m_ParticlePool){
            if (!particle.active){
                continue;
            }

            if (particle.lifeRemaining <= 0.0f){
                particle.active = false;
                continue;
            }

            particle.lifeRemaining -= ts;
            glm::vec2 particleOldPos = particle.position;
            particle.position += particle.velocity * (float)ts;
            //particle.lifeRemaining -= std::abs(glm::distance(particle.position,particleOldPos));
            particle.rotation += 0.01f * ts;
        }
    }

    void ParticleSystem::onRender(const GEOGL::OrthographicCamera& camera){

        GEOGL::Renderer2D::beginScene(camera);

        for (auto& particle : m_ParticlePool){
            if (!particle.active)
                continue;

            // Fade away particles
            float life = particle.lifeRemaining / particle.lifeTime;
            glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
            //color.a = color.a * life;

            float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

            GEOGL::Renderer2D::drawRotatedQuad({{particle.position.xy, .9f}, {size, size}, color}, particle.rotation);

        }

        GEOGL::Renderer2D::endScene();

    }

    void ParticleSystem::emit(const ParticleProperties& particleProps)
    {
        Particle& particle = m_ParticlePool[m_PoolIndex];
        particle.active = true;
        particle.position = particleProps.position;
        particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

        // Velocity
        particle.velocity = particleProps.velocity;
        float randoma = Random::Float();
        float random = (Random::Float() - 0.5f);
        float variationX = particleProps.velocityVariation.x * (Random::Float() - 0.5f);
        particle.velocity.x += particleProps.velocityVariation.x * (Random::Float() - 0.5f);
        particle.velocity.y += particleProps.velocityVariation.y * (Random::Float() - 0.5f);

        // Color
        particle.colorBegin = particleProps.colorBegin;
        particle.colorEnd = particleProps.colorEnd;

        particle.lifeTime = particleProps.lifeTime;
        particle.lifeRemaining = particleProps.lifeTime;
        particle.sizeBegin = particleProps.sizeBegin + particleProps.sizeVariation * (Random::Float() - 0.5f);
        particle.sizeEnd = particleProps.sizeEnd;

        m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
    }

}