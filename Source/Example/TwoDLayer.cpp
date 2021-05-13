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
#include "TwoDLayer.hpp"
#include "../Platform/OpenGL/Rendering/OpenGLShader.hpp"
#include <limits>



namespace Example{

    static uint64_t frameCount = 0;
    static float minFPS = std::numeric_limits<float>::max();
    static float maxFPS = 0; // this can be zero as we shouldn't have negative FPS
    static float totalFrameTime = 0;

    static glm::mat4 scaleOne;

    TwoDLayer::TwoDLayer() :
    /* The name of the layer */
    GEOGL::Layer("2D Layer"),
    /* The position of the camera */
    m_CameraPosition(glm::vec3({0.0f,0.0f,0.0f})){
        /* Now that we are properly in the constructor, we set up the camera and everything else */
        m_Camera.setPosition(m_CameraPosition);

        scaleOne = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));

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

            std::vector<float> vertices = {-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
                                           0.5f,   -0.5f, 0.0f, 1.0f, 0.0f,
                                           0.5f,   0.5f,  0.0f, 1.0f, 1.0f,
                                           -0.5f,  0.5f,  0.0f, 0.0f, 1.0f};

            auto vertexBuffer = GEOGL::VertexBuffer::create(vertices);
            {
                vertexBuffer->setLayout({
                                                {GEOGL::ShaderDataType::FLOAT3, "a_Position"},
                                                {GEOGL::ShaderDataType::FLOAT2, "a_TextureCoord"}
                                        });
            }

            std::vector<uint32_t> indices = {0,1,2,0,2,3};
            auto indexBuffer = GEOGL::IndexBuffer::create(indices);

            m_VertexArraySquare->addVertexBuffer(vertexBuffer);
            m_VertexArraySquare->setIndexBuffer(indexBuffer);

        }

        {
            m_PerVertexShader = GEOGL::Shader::create("Resources/Shaders/PerVertexColor");
            m_FlatColorShader = GEOGL::Shader::create("Resources/Shaders/FlatColorShader");
            m_TextureShader = GEOGL::Shader::create("Resources/Shaders/BasicTexture");

            m_TextureShader->bind();
            std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(m_TextureShader)->uploadUniformInt("u_Texture", 0);
        }

        /*
         * Load Textures
         */
        {

            m_CheckerboardTexture = GEOGL::Texture2D::create("Resources/Textures/Checkerboard.png");
            m_ChernoLogo = GEOGL::Texture2D::create("Resources/Textures/ChernoLogo.png");
        }

    }

    void TwoDLayer::onAttach() {

        m_Scalepointoneone = glm::scale(glm::mat4(1.0f), {.10,.10,.0});

    }

    void TwoDLayer::onDetach() {

    }

    void TwoDLayer::onUpdate(GEOGL::TimeStep timeStep) {

        pollCameraMovement(timeStep);

        GEOGL::Renderer::beginScene(m_Camera);

        m_FlatColorShader->bind();
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(m_FlatColorShader)->uploadUniformFloat3("u_Color", m_SquareColor);

        for(unsigned int i=0; i<20; ++i){
            for(unsigned int j=0; j<20; ++j) {
                glm::vec3 pos((j * .11f)-1, (i * .11f)-1, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * m_Scalepointoneone;
                GEOGL::Renderer::submit(m_FlatColorShader, m_VertexArraySquare, transform);
            }
        }

        // hide triangle
        m_TextureShader->bind();
        std::dynamic_pointer_cast<GEOGL::Platform::OpenGL::Shader>(m_TextureShader)->uploadUniformFloat3("u_Color", glm::vec3(0.8f,0.8f,0.8f));

        m_CheckerboardTexture->bind(0);
        GEOGL::Renderer::submit(m_TextureShader,m_VertexArraySquare,scaleOne);
        m_ChernoLogo->bind(0);
        GEOGL::Renderer::submit(m_TextureShader,m_VertexArraySquare,scaleOne);

        GEOGL::Renderer::endScene();

    }

    void TwoDLayer::onImGuiRender(GEOGL::TimeStep timeStep) {

        totalFrameTime += timeStep;
        /* Only run the fps min and max afterwords to give time to stabalize */
        if(frameCount++ > 45) {

            auto dimensions = GEOGL::Application::get().getWindow().getDimensions();

            float fps = 1.0f / (timeStep.getSeconds());
            double averageFPS = 1.0f/(totalFrameTime/frameCount);
            minFPS = (fps < minFPS) ? fps : minFPS;
            maxFPS = (fps > maxFPS) ? fps : maxFPS;


            ImGui::Begin("Debug Info");
            ImGui::SetWindowFontScale(2.0);
            ImGui::Text("Window size %d x %d", dimensions.x, dimensions.y);
            ImGui::Text("Aspect Ratio %f", (float)dimensions.x/(float)dimensions.y);
            ImGui::Text("VSync Enabled: %s", (GEOGL::Application::get().getWindow().isVSync()) ? "TRUE" : "FALSE");
            ImGui::Text("FrameTime: %.2f ms", timeStep.getMilliseconds());
            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Min FPS: %.2f", minFPS);
            ImGui::Text("Max FPS: %.2f", maxFPS);
            ImGui::Text("Average FPS: %.2f", averageFPS);
            ImGui::Text("Frame Count: %llu", frameCount);
            ImGui::Text("Total Frame Time: %.2f s", totalFrameTime);
            ImGui::Text("Total Memory Allocations: %zu", GEOGL::getNumberAllocations());
            ImGui::Text("Total Memory Deallocations: %zu", GEOGL::getNumberDeallocations());
            ImGui::Text("Total Memory Allocated: %.2f MB", GEOGL::getMegabytesAllocated());
            ImGui::Text("Total Memory Deallocated: %.2f MB", GEOGL::getMegabytesDeallocated());

            if(ImGui::Button("Clear FPS Information")){
                fps=0;
                minFPS = std::numeric_limits<float>::max();
                maxFPS = 0;
                averageFPS = 0;
                frameCount = 0;
                totalFrameTime = 0;
            }
            ImGui::End();
        }else{
            ImGui::Begin("Preparing Debug Info");
            ImGui::SetWindowFontScale(2.0);
            ImGui::Text("Waiting for FPS to stabilize.");
            ImGui::End();
        }

        ImGui::Begin("Settings");
        ImGui::SetWindowFontScale(2.0);
        ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
        ImGui::End();

    }

    void TwoDLayer::onEvent(GEOGL::Event& event) {

        GEOGL::EventDispatcher dispatcher(event);

        /* Dispatch the event to the appropriate functions */
        dispatcher.dispatch<GEOGL::KeyPressedEvent>(GEOGL_BIND_EVENT_FN(TwoDLayer::onKeyPressedEvent));
        dispatcher.dispatch<GEOGL::WindowResizeEvent>(GEOGL_BIND_EVENT_FN(TwoDLayer::onWindowResizeEvent));

    }

    bool TwoDLayer::onKeyPressedEvent(GEOGL::KeyPressedEvent& event){

        /* Allow vSync toggle with ctrl + v */
        if(event.getKeyCode() == GEOGL::Key::V && GEOGL::Input::isKeyPressed(GEOGL::Key::LeftControl)){
            auto& window = GEOGL::Application::get().getWindow();

            bool vSync = !window.isVSync();
            window.setVSync(vSync);

            return true;

        }

        if(event.getKeyCode() == GEOGL::Key::Escape){
            GEOGL::WindowCloseEvent windowCloseEvent = GEOGL::WindowCloseEvent();
            GEOGL::Application::get().onEvent(windowCloseEvent);
        }

        return false;

    }

    bool TwoDLayer::onWindowResizeEvent(GEOGL::WindowResizeEvent &windowResizeEvent) {

        /* Calculate the new OrthographicBounds and set it to the camera. This keeps
         * a square a square no matter how much we resize the window */
        GEOGL::OrthographicCamera::OrthographicBounds bounds = GEOGL::OrthographicCamera::calculateBestOrthographicBounds(
                {windowResizeEvent.getWidth(), windowResizeEvent.getHeight()});

        m_Camera.setOrthographicBounds(bounds);

        return false;

    }

    void TwoDLayer::pollCameraMovement(GEOGL::TimeStep& timeStep) {

        glm::vec3 deltaPosition(0.0f);

        /* If dpad or q or e is pressed, translate or rotate */
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::Left)) {
            deltaPosition.x -= m_CameraSpeed * timeStep;
        }
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::Right)) {
            deltaPosition.x += m_CameraSpeed * timeStep;
        }
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::Down)) {
            deltaPosition.y -= m_CameraSpeed * timeStep;
        }
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::Up)) {
            deltaPosition.y += m_CameraSpeed * timeStep;
        }
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::Q)) {
            m_CameraRotation -= m_CameraRotSpeed * timeStep;
        }
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::E)) {
            m_CameraRotation += m_CameraRotSpeed * timeStep;
        }
        /* Double the speed */
        if (GEOGL::Input::isKeyPressed(GEOGL::Key::LeftShift) || GEOGL::Input::isKeyPressed(GEOGL::Key::RightShift)) {
            deltaPosition *= glm::vec3(2.0f);
        }

        /* Now, update the camera position */
        m_CameraPosition += deltaPosition;

        /* if c is pressed, center everything */
        if(GEOGL::Input::isKeyPressed(GEOGL::Key::C)){
            m_CameraPosition = glm::vec3(0.0f);
            m_CameraRotation = 0;
        }


        /* Now upload it to the camera */
        m_Camera.setPosition(m_CameraPosition);
        m_Camera.setRotationZ(m_CameraRotation);

    }

}