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
#include <ImGui/imgui.h>
#include <GEOGL/Platform/OpenGL.hpp>

namespace SandboxApp{

    static glm::mat4 scalePointOneOne;
    Layer2D::Layer2D() :
            m_OrthographicCameraController(0){
    };

    void Layer2D::onAttach() {

        scalePointOneOne = glm::scale(glm::mat4(1.0f), {.10,.10,.0});

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Layer2D - Sandbox";

        m_ChernoLogo = GEOGL::Texture2D::create("Resources/Textures/ChernoLogo.png");
        m_Checkerboard = GEOGL::Texture2D::create("Resources/Textures/Checkerboard.png");

    }

    void Layer2D::onDetach() {


    }

    void Layer2D::onUpdate(GEOGL::TimeStep timeStep) {

        m_OrthographicCameraController.onUpdate(timeStep);

        GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());

        //GEOGL::Renderer2D::drawQuad({0,0,1}, {1,1}, {1-m_SquareColor.r, 1-m_SquareColor.g, 1-m_SquareColor.b, m_SquareColor.a}, 45);
        GEOGL::Renderer2D::drawQuad({0,0,-.9}, {10,10}, m_Checkerboard, 10, 0, {1-m_SquareColor.r, 1-m_SquareColor.g, 1-m_SquareColor.b, m_SquareColor.a});
        GEOGL::Renderer2D::drawQuad({0,0,.5}, {1,1}, m_Checkerboard,1, 45);
        GEOGL::Renderer2D::drawQuad({0,0}, {sqrt(2), sqrt(2)}, m_SquareColor);
        GEOGL::Renderer2D::drawQuad({-.05,0,1}, {1,1}, m_ChernoLogo);

        /* Upload flat color shader color */
        /*{

            // TODO: Shader::setMat4, shader::setFloat4
            m_Shader->bind();
            std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(m_Shader)->uploadUniformFloat4("u_Color", m_SquareColor);
        }*/

        /*for(unsigned int i=0; i<20; ++i){
            for(unsigned int j=0; j<20; ++j) {
                glm::vec3 pos((j * .11f)-1, (i * .11f)-1, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scalePointOneOne;
                GEOGL::Renderer::submit(m_Shader, m_SquareVA, transform);
            }
        }*/
        //GEOGL::Renderer::submit(m_Shader, m_SquareVA);

        GEOGL::Renderer2D::endScene();

    }

    void Layer2D::onImGuiRender(GEOGL::TimeStep timeStep) {

        ImGui::Begin("Color Picker");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();

    }

    void Layer2D::onEvent(GEOGL::Event &event) {

        m_OrthographicCameraController.onEvent(event);

    }
}
