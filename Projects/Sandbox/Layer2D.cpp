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

#define GEOGL_SWIZZLE
#include "Layer2D.hpp"
#include <GEOGL/Core.hpp>
#include <ImGui/imgui.h>
#include <GEOGL/Platform/OpenGL.hpp>

namespace SandboxApp{

    static float rotation = 0;

    Layer2D::Layer2D() :
            m_OrthographicCameraController(0){
    };

    void Layer2D::onAttach() {
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Layer2D - Sandbox";

        m_ChernoLogo = GEOGL::Texture2D::create("Resources/Textures/ChernoLogo.png");
        m_Checkerboard = GEOGL::Texture2D::create("Resources/Textures/Checkerboard.png");
        m_Sandman = GEOGL::Texture2D::create("Resources/Textures/Sandman.png");

        m_ParticleSystem = GEOGL::createScope<ParticleSystem>();

    }

    void Layer2D::onDetach() {


    }

    void Layer2D::onUpdate(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();

        GEOGL::Renderer2D::resetStats();

        m_OrthographicCameraController.onUpdate(timeStep);

        /*GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());


        rotation += timeStep * 20;

        /*{

            GEOGL::Renderer2D::drawQuad({{0,  0, -.9}, {100, 100}, {1 - m_SquareColor.r, 1 - m_SquareColor.g, 1 - m_SquareColor.b, m_SquareColor.a}, 100}, m_Checkerboard);
            GEOGL::Renderer2D::drawRotatedQuad({{0, 0, .2}, {1, 1}, {1,1,1,1}}, m_Checkerboard, glm::radians(rotation));
            GEOGL::Renderer2D::drawQuad({{0,0,0},{sqrt(2), sqrt(2)}, m_SquareColor});
            //GEOGL::Renderer2D::drawQuad({{5,5,0},{sqrt(2), sqrt(2)}, m_SquareColor});

            GEOGL::Renderer2D::drawQuad({{-.05, 0, .3}, {1, 1}}, m_ChernoLogo);

            GEOGL::Renderer2D::drawQuad({{-3,0,0}, {2,2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{-3,-3,0}, {2,2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{0, -3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, -3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, 0, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, 3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{0, 3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{-3, 3, 0},{2,  2}}, m_Sandman);

            //for(int i=0; i<0; i++){
              //  GEOGL::Renderer2D::drawQuad({{-3, 3, 0},{2,  2}}, m_Sandman);
            //}

        }

        GEOGL::Renderer2D::endScene();*/

        /* emit one particle */
        {
            ParticleProperties properties{};
            properties.position = {0, 0};
            properties.velocity = {(Random::Float()-.5)*2, (Random::Float()-.5)*2};
            properties.velocityVariation = {Random::Float()/5, Random::Float()/5};
            properties.sizeVariation = Random::Float()/5;
            properties.sizeBegin = 0.2f;
            properties.sizeEnd = 0;
            properties.lifeTime = std::max(1.0f,Random::Float() * 4);
            properties.colorBegin = {.2,.2f,.8f,1};
            properties.colorEnd = {0.6f, 0.2f,0.2f,1};
            m_ParticleSystem->emit(properties);
        }
        {
            ParticleProperties properties{};
            properties.position = {0, 0};
            properties.velocity = {(Random::Float()-.5)*2, (Random::Float()-.5)*2};
            properties.velocityVariation = {Random::Float()/5, Random::Float()/5};
            properties.sizeVariation = Random::Float()/5;
            properties.sizeBegin = 0.2f;
            properties.sizeEnd = 0;
            properties.lifeTime = std::max(1.0f,Random::Float() * 4);
            properties.colorBegin = {.2,.2f,.8f,1};
            properties.colorEnd = {0.6f, 0.2f,0.2f,1};
            m_ParticleSystem->emit(properties);
        }
        {
            ParticleProperties properties{};
            properties.position = {0, 0};
            properties.velocity = {(Random::Float()-.5)*2, (Random::Float()-.5)*2};
            properties.velocityVariation = {Random::Float()/5, Random::Float()/5};
            properties.sizeVariation = Random::Float()/5;
            properties.sizeBegin = 0.2f;
            properties.sizeEnd = 0;
            properties.lifeTime = std::max(1.0f,Random::Float() * 4);
            properties.colorBegin = {.2,.2f,.8f,1};
            properties.colorEnd = {0.6f, 0.2f,0.2f,1};
            m_ParticleSystem->emit(properties);
        }

        m_ParticleSystem->onUpdate(timeStep);

        m_ParticleSystem->onRender(m_OrthographicCameraController.getCamera());

    }

    void Layer2D::onImGuiRender(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();

        ImGui::Begin("Color Picker");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();

    }

    void Layer2D::onEvent(GEOGL::Event &event) {
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicCameraController.onEvent(event);

    }
}
