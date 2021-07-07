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

namespace SandboxApp{

    static float rotation = 0;

    Layer2D::Layer2D() :
            m_OrthographicCameraController(0){
    };

    void Layer2D::onAttach() {
        GEOGL_PROFILE_FUNCTION();

        GEOGL::FramebufferSpecification framebufferSpecification{};
        framebufferSpecification.width = GEOGL::Application::get().getWindow().getWidth();
        framebufferSpecification.height = GEOGL::Application::get().getWindow().getHeight();
        m_Framebuffer = GEOGL::Framebuffer::create(framebufferSpecification);

        m_OrthographicCameraController = GEOGL::OrthographicCameraController(GEOGL::Application::get().getWindow().getDimensions());
        m_DebugName = "Layer2D - Sandbox";

        m_ChernoLogo = GEOGL::Texture2D::create("SandboxResources/Textures/ChernoLogo.png");
        m_Checkerboard = GEOGL::Texture2D::create("SandboxResources/Textures/Checkerboard.png");
        m_Sandman = GEOGL::Texture2D::create("SandboxResources/Textures/Sandman.png");

        m_ParticleSystem = GEOGL::createScope<ParticleSystem>(10000);

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
        m_Framebuffer->bind();
        GEOGL::Renderer::setClearColor({0.1f,0.1f,0.1f,1.0f});
        GEOGL::RenderCommand::clear();


        m_OrthographicCameraController.onUpdate(timeStep);

        GEOGL::Renderer2D::beginScene(m_OrthographicCameraController.getCamera());


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

        GEOGL::Renderer2D::endScene();

        /* emit 50 particle *
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

        m_ParticleSystem->onUpdate(timeStep);

        m_ParticleSystem->onRender(m_OrthographicCameraController.getCamera()); */

        m_Framebuffer->unbind();

    }

    void Layer2D::onImGuiRender(GEOGL::TimeStep timeStep) {

        GEOGL_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar()){

            /* File Menu */
            if (ImGui::BeginMenu("File")){
                if(ImGui::MenuItem("Exit")){
                    GEOGL::Application::get().close();
                };
                ImGui::EndMenu();
            }

            /* About Menu */
            if(ImGui::BeginMenu("About")){
                if(ImGui::MenuItem("Licenses")){
                    GEOGL_INFO_NOSTRIP("Licenses coming Soon!");
                }
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }

        uint32_t textureID = m_Framebuffer->getColorAttachmentRendererID();
        ImVec2 framebufferDimensions = {(float)m_Framebuffer->getFramebufferSpecification().width, (float)m_Framebuffer->getFramebufferSpecification().height};

#pragma warning (push)
#pragma warning (disable:4312)
        ImGui::Begin("Window");
        ImGui::Image(reinterpret_cast<void*>(textureID),framebufferDimensions);
        ImGui::End();
        #pragma warning (pop)
        ImGui::End();

        //static bool show = true;
        //ImGui::ShowDemoWindow(&show);


    }

    void Layer2D::onEvent(GEOGL::Event &event) {
        GEOGL_PROFILE_FUNCTION();

        m_OrthographicCameraController.onEvent(event);

    }
}
