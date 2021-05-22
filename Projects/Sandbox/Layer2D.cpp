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
#include <ImGui/imgui.h>
#include <GEOGL/Platform/OpenGL.hpp>

namespace SandboxApp{

    static glm::mat4 scalePointOneOne;
    Layer2D::Layer2D() :
            m_OrthographicCameraController(0){
    };

    void Layer2D::onAttach() {
        GEOGL_PROFILE_FUNCTION();

        scalePointOneOne = glm::scale(glm::mat4(1.0f), {.10,.10,.0});

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Layer2D - Sandbox";

        m_ChernoLogo = GEOGL::Texture2D::create("Resources/Textures/ChernoLogo.png");
        m_Checkerboard = GEOGL::Texture2D::create("Resources/Textures/Checkerboard.png");
        m_Sandman = GEOGL::Texture2D::create("Resources/Textures/Sandman.png");

    }

    void Layer2D::onDetach() {


    }

    void Layer2D::onUpdate(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();

        m_OrthographicCameraController.onUpdate(timeStep);

        GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());

        {

            GEOGL::Renderer2D::drawQuad({{0,  0, -.9}, {10, 10}, {1 - m_SquareColor.r, 1 - m_SquareColor.g, 1 - m_SquareColor.b, m_SquareColor.a}, 10, 0}, m_Checkerboard);
            GEOGL::Renderer2D::drawQuad({{0, 0, .5}, {1, 1}, {1,1,1,1}, .5, glm::radians(45.0)}, m_Checkerboard);
            GEOGL::Renderer2D::drawQuad({{0,0,0},{sqrt(2), sqrt(2)}, m_SquareColor});
            GEOGL::Renderer2D::drawQuad({{-.05, 0, 1}, {1, 1}}, m_ChernoLogo);

            GEOGL::Renderer2D::drawQuad({{-3,0,0}, {2,2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{-3,-3,0}, {2,2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{0, -3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, -3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, 0, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{3, 3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{0, 3, 0},{2, 2}}, m_Sandman);
            GEOGL::Renderer2D::drawQuad({{-3, 3, 0},{2,  2}}, m_Sandman);
        }

        GEOGL::Renderer2D::endScene();

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
