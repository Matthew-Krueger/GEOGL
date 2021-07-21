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


#include "Layer2D.hpp"
#include <GEOGL/Core.hpp>
#include <GEOGL/IO.hpp>
#include <ImGui/imgui.h>
#include <GEOGL/Platform/OpenGL.hpp>

/* #define FLIRTY_PARTICLES
 * to define that the particle system should emit particles in the shape of a heart
 */
#define FLIRTY_PARTICLES
namespace SandboxApp{

    static float rotation = 0;

    static glm::vec4* particleEmitters = nullptr;
    static size_t particleEmittersLength = 500;

    Layer2D::Layer2D() :
            m_OrthographicCameraController(0){
    };

    void Layer2D::onAttach() {
        GEOGL_PROFILE_FUNCTION();

#ifdef FLIRTY_PARTICLES
        GEOGL_INFO("Since flirty particles are selected, generating particle emitter positions using a cardioid curve. (Yay math)");
        particleEmitters = new glm::vec4[particleEmittersLength];

        /* generate the particle emitter positions using a cardioid curve */
        const double startT = 0;
        const double endT = 2 * glm::pi<double>();


        glm::mat4 transform = glm::scale(glm::mat4(1.0f), {0.05f,0.05f,0.1f});

        /* Generate the heart curve for flirty particles in parallel. */
#pragma omp parallel for
        for(int i=0; i<particleEmittersLength; ++i){
            double t = (((double)i/(double)particleEmittersLength) * (endT-startT))+startT;
            particleEmitters[i].x =(float) (16 * std::pow(std::sin(t),3));
            particleEmitters[i].y =(float) ((13 * std::cos(t)) - (5*std::cos(2*t)) - (2*std::cos(3*t)) - (std::cos(4*t)));
            particleEmitters[i].z =0.0f;
            particleEmitters[i].a =0.0f;

            /* shrink the emitter coords into a space that fits on the screen */
            particleEmitters[i] = transform * particleEmitters[i];
        }

#endif

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Layer2D - Sandbox";

        m_ChernoLogo = GEOGL::Texture2D::create("SandboxResources/Textures/ChernoLogo.png");
        m_Checkerboard = GEOGL::Texture2D::create("SandboxResources/Textures/Checkerboard.png");
        m_Sandman = GEOGL::Texture2D::create("SandboxResources/Textures/Sandman.png");

        m_ParticleSystem = GEOGL::createScope<ParticleSystem>(100000);

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("SandboxResources/Fonts/OpenSans/OpenSans-Regular.ttf", 18.0f);
        io.Fonts->Build();
        io.Fonts->AddFontDefault();

    }

    void Layer2D::onDetach() {


    }

    void Layer2D::onUpdate(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();


        GEOGL::Renderer2D::resetStats();
        GEOGL::Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});
        GEOGL::RenderCommand::clear();


        m_OrthographicCameraController.onUpdate(timeStep);

        /*GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());


        rotation += timeStep * 20;

        {

            GEOGL::Renderer2D::drawQuad({{0,  0, -.9}, {100, 100}, {1 - m_SquareColor.r, 1 - m_SquareColor.g, 1 - m_SquareColor.b, m_SquareColor.a}, 100}, m_Checkerboard);
            GEOGL::Renderer2D::drawRotatedQuad({{0, 0, .2}, {1, 1}, {1,1,1,1}}, m_Checkerboard, glm::radians(rotation));
            GEOGL::Renderer2D::drawRotatedQuad({{0,0,0},{sqrt(2), sqrt(2)}, m_SquareColor}, glm::radians(180.0f));

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

        GEOGL::Renderer2D::endScene(); */

#ifdef FLIRTY_PARTICLES

        /* Emit Flirty particles, for obvious reasons, according to the heart curve calculated on setup */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::H)) {

            /* since h is pressed, render flirty particles */
            for(int i=0; i<particleEmittersLength; ++i) {

                ParticleProperties properties{};
                properties.position = {(particleEmitters[i]).x, (particleEmitters[i]).y};
                properties.velocity = {0.0f,.5f};//{0, .7};
                properties.velocityVariation = {0.5f,.5f};//{Random::Float(), Random::Float()/2};
                properties.sizeVariation = 0.0;
                properties.sizeBegin = 0.1f;
                properties.sizeEnd = 0.01f;
                properties.lifeTime = 0.5f;//std::max(0.05f,Random::Float()*2);
                properties.colorBegin = {.9,.01f,.1f,1};
                properties.colorEnd = {0.4f, 0.01f,0.8f,1};
                m_ParticleSystem->emit(properties);

            }


        }

#else

        /* emit 50 particle */

        if(GEOGL::Input::isMouseButtonPressed(GEOGL::Mouse::ButtonLeft)){
            glm::vec2 mousePos = GEOGL::Input::getMousePosition();
            glm::ivec2 windowDimensions = GEOGL::Application::get().getWindow().getDimensions();

            auto& bounds = m_OrthographicCameraController.getCamera().getProjectionBounds();
            auto& cameraPosition = m_OrthographicCameraController.getCamera().getPosition();

            glm::vec2 worldSpaceEmitterPosition = {0,0};
            worldSpaceEmitterPosition.x = ((mousePos.x/(float)windowDimensions.x) * bounds.getWidth()) - bounds.getWidth() * 0.5f;
            worldSpaceEmitterPosition.y = bounds.getHeight()*0.5f - ((mousePos.y/(float)windowDimensions.y) * bounds.getHeight());
            worldSpaceEmitterPosition = worldSpaceEmitterPosition + cameraPosition.xy;

            for(int i=0; i<50; ++i){
                ParticleProperties properties{};
                properties.position = worldSpaceEmitterPosition;
                properties.velocity = {0.0f,0.0f};//{0, .7};
                properties.velocityVariation = {5,5};//{Random::Float(), Random::Float()/2};
                properties.sizeVariation = 0.0;
                properties.sizeBegin = 0.2f;
                properties.sizeEnd = 0.01f;
                properties.lifeTime = 1.0f;//std::max(0.05f,Random::Float()*2);
                properties.colorBegin = {.5,.5f,.8f,1};
                properties.colorEnd = {0.6f, 0.6f,0.2f,1};
                m_ParticleSystem->emit(properties);
            }
        }
#endif

        m_ParticleSystem->onUpdate(timeStep);

        m_ParticleSystem->onRender(m_OrthographicCameraController.getCamera());


    }

    void Layer2D::onImGuiRender(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();

    }

    void Layer2D::onEvent(GEOGL::Event &event) {
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicCameraController.onEvent(event);

    }
}
