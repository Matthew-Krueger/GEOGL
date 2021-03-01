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


#include <ImGui/imgui.h>
#include "3DLayer.hpp"
#include <limits>


namespace Example{

    static uint64_t frameCount = 0;
    static float minFPS = std::numeric_limits<float>::max();
    static float maxFPS = 0; // this can be zero as we shouldn't have negative FPS

    TwoDLayer::TwoDLayer() :
        GEOGL::Layer("2D Layer"),
        m_Camera({-1.9,1.9,-.9,.9}),
        m_CameraPosition(glm::vec3({0.0f,0.0f,0.0f})){



        /* set up the triangle */
        {

            /* Create a vertex array */
            m_VertexArrayTrianglePerVColor = GEOGL::VertexArray::create();

            std::vector<float> vertices = {-0.5f,-0.5f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f,
                                           0.5f, -0.5f, 0.0f, 0.0f, 0.8f, 0.0f, 1.0f,
                                           0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.8f, 1.0f};

            auto vertexBuffer = GEOGL::VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {GEOGL::ShaderDataType::FLOAT3, "a_Position"},
                                                {GEOGL::ShaderDataType::FLOAT4, "a_Color"}
                                        });
            }

            std::vector<uint32_t> indices = {0,1,2};
            auto indexBuffer = GEOGL::IndexBuffer::create(indices);

            m_VertexArrayTrianglePerVColor->addVertexBuffer(vertexBuffer);
            m_VertexArrayTrianglePerVColor->setIndexBuffer(indexBuffer);

        }

        /* set up the square */
        {

            /* Create a vertex array */
            m_VertexArraySquare = GEOGL::VertexArray::create();

            std::vector<float> vertices = {-0.75f, -0.75f, 0.0f,
                                           0.75f, -0.75f, 0.0f,
                                           0.75f,  0.75f, 0.0f,
                                           -0.75f,  0.75f, 0.0f};

            auto vertexBuffer = GEOGL::VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {GEOGL::ShaderDataType::FLOAT3, "a_Position"},
                                        });
            }

            std::vector<uint32_t> indices = {0,1,2,0,2,3};
            auto indexBuffer = GEOGL::IndexBuffer::create(indices);

            m_VertexArraySquare->addVertexBuffer(vertexBuffer);
            m_VertexArraySquare->setIndexBuffer(indexBuffer);

        }

        {
            std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ProjectionViewMatrix;

			out vec4 v_Position;
			void main()
			{
				v_Position = a_Color;
				gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
			}
		)";

            std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			in vec4 v_Position;
			void main()
			{
				color = v_Position;
			}
		)";

            m_PerVertexShader = GEOGL::Shader::create(vertexSrc, fragmentSrc);
        }

        {
            std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
            uniform mat4 u_ProjectionViewMatrix;

			void main()
			{
				gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
			}
		)";

            std::string fragmentSrc = R"(
			#version 330 core


			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.03,0.03,0.2,1.0);
			}
		)";

            m_BlueShader = GEOGL::Shader::create(vertexSrc, fragmentSrc);
        }

    }

    void TwoDLayer::onAttach() {

    }

    void TwoDLayer::onDetach() {

    }

    void TwoDLayer::onUpdate(GEOGL::TimeStep timeStep) {

        pollCameraMovement(timeStep);

        GEOGL::Renderer::beginScene(m_Camera);

        GEOGL::Renderer::submit(m_BlueShader, m_VertexArraySquare);
        GEOGL::Renderer::submit(m_PerVertexShader, m_VertexArrayTrianglePerVColor);

        GEOGL::Renderer::endScene();

    }

    void TwoDLayer::onImGuiRender(GEOGL::TimeStep timeStep) {

        /* Only run the fps min and max afterwords to give time to stabalize */
        if(frameCount++ > 25) {
            float fps = 1.0f / (timeStep.getSeconds());
            minFPS = (fps < minFPS) ? fps : minFPS;
            maxFPS = (fps > maxFPS) ? fps : maxFPS;


            ImGui::Begin("Debug Info");
            ImGui::SetWindowFontScale(2.0);
            ImGui::Text("VSync Enabled: %s", (GEOGL::Application::get().getWindow().isVSync()) ? "TRUE" : "FALSE");
            ImGui::Text("FrameTime: %.2f ms", timeStep.getMilliseconds());
            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Min FPS: %.4f", minFPS);
            ImGui::Text("Max FPS: %.4f", maxFPS);
            ImGui::Text("Frame Count: %lu", frameCount);
            ImGui::End();
        }else{
            ImGui::Begin("Preparing Debug Info");
            ImGui::SetWindowFontScale(2.0);
            ImGui::Text("Waiting for FPS to stabilize.");
            ImGui::End();
        }


    }

    void TwoDLayer::onEvent(GEOGL::Event& event) {

        GEOGL::EventDispatcher dispatcher(event);

        dispatcher.dispatch<GEOGL::KeyPressedEvent>(GEOGL_BIND_EVENT_FN(TwoDLayer::onKeyPressedEvent));

    }

    bool TwoDLayer::onKeyPressedEvent(GEOGL::KeyPressedEvent& event){

        /* Allow vSync toggle with ctrl + v */
        if(event.getKeyCode() == GEOGL::Key::V && GEOGL::Input::isKeyPressed(GEOGL::Key::LeftControl)){
            auto& window = GEOGL::Application::get().getWindow();

            bool vSync = !window.isVSync();
            window.setVSync(vSync);

            return true;

        }

        return false;

    }

    void TwoDLayer::pollCameraMovement(GEOGL::TimeStep& timeStep) {

        glm::vec3 deltaPosition(0.0f);

        /* If dpad or q or e is pressed, translate or rotate */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::Left)){
            deltaPosition.x -= m_CameraSpeed * timeStep;
        }
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::Right)){
            deltaPosition.x += m_CameraSpeed * timeStep;
        }
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::Down)){
            deltaPosition.y -= m_CameraSpeed * timeStep;
        }
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::Up)){
            deltaPosition.y += m_CameraSpeed * timeStep;
        }
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::Q)){
            m_CameraRotation -= m_CameraRotSpeed * timeStep;
        }
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::E)){
            m_CameraRotation += m_CameraRotSpeed * timeStep;
        }

        /* Double the speed */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::LeftShift) || GEOGL::Input::isKeyPressed(GEOGL::Key::RightShift)){
            deltaPosition.xyz *= glm::vec3(2.0f);
        }

        /* Now, update the camera position */
        m_CameraPosition.xyz += deltaPosition.xyz;

        /* If C is pressed, reset the position */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::C)){
            m_CameraPosition.xyz = 0;
            m_CameraRotation = 0;
        }

        /* Now upload it to the camera */
        m_Camera.setPosition(m_CameraPosition);
        m_Camera.setRotationZ(m_CameraRotation);

    }

}